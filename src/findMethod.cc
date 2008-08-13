#include "PhysicsTools/Utilities/src/findMethod.h"
#include "PhysicsTools/Utilities/src/ErrorCodes.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "Reflex/Base.h"
#include "Reflex/TypeTemplate.h"

using namespace ROOT::Reflex;
using namespace std;
using reco::parser::AnyMethodArgument;

//Checks for errors which show we got the correct function be we just can't use it
static bool fatalErrorCondition(int iError)
{
   return (reco::parser::kIsNotPublic==iError ||
      reco::parser::kIsStatic==iError ||
      reco::parser::kIsFunctionAddedByROOT==iError ||
      reco::parser::kIsConstructor==iError ||
      reco::parser::kIsDestructor==iError ||
      reco::parser::kIsOperator==iError);
   
}
namespace reco {
  int checkMethod(const ROOT::Reflex::Member & mem, const std::vector<AnyMethodArgument> &args, std::vector<AnyMethodArgument> &fixuppedArgs) {
    int casts = 0;
    if (mem.IsConstructor()) return -1*parser::kIsConstructor;
    if (mem.IsDestructor()) return -1*parser::kIsDestructor;
    if (mem.IsOperator()) return -1*parser::kIsOperator;
    if (! mem.IsPublic()) return -1*parser::kIsNotPublic;
    if (mem.IsStatic()) return -1*parser::kIsStatic;
    if ( ! mem.TypeOf().IsConst() ) return -1*parser::kIsNotConst;
    if (mem.Name().substr(0, 2) == "__") return -1*parser::kIsFunctionAddedByROOT;
    size_t minArgs = mem.FunctionParameterSize(true), maxArgs = mem.FunctionParameterSize(false);
    //std::cerr << "\nMETHOD " << mem.Name() << " of " << mem.DeclaringType().Name() 
    //    << ", min #args = " << minArgs << ", max #args = " << maxArgs 
    //    << ", args = " << args.size() << std::endl;
    if ((args.size() < minArgs) || (args.size() > maxArgs)) return -1*parser::kWrongNumberOfArguments;
    if (!args.empty()) {
        Type t = mem.TypeOf();
        for (size_t i = 0; i < args.size(); ++i) { 
            std::pair<AnyMethodArgument,int> fixup = boost::apply_visitor( reco::parser::AnyMethodArgumentFixup(t.FunctionParameterAt(i)), args[i] );
            //std::cerr << "\t ARG " << i << " type is " << t.FunctionParameterAt(i).Name() << " conversion = " << fixup.second << std::endl; 
            if (fixup.second >= 0) { 
                fixuppedArgs.push_back(fixup.first);
                casts += fixup.second;
            } else { 
                fixuppedArgs.clear(); 
                return -1*parser::kWrongArgumentType;
            }
        }
    }
    //std::cerr << "\nMETHOD " << mem.Name() << " of " << mem.DeclaringType().Name() << " matched with " << casts << " casts" << std::endl;
    return casts;
  }

  pair<Member, bool> findMethod(const Type & t, const string & name, const std::vector<AnyMethodArgument> &args, std::vector<AnyMethodArgument> &fixuppedArgs, int& oError) {
     oError = parser::kNameDoesNotExist;
    Type type = t; 
    if (! type)  
      throw edm::Exception(edm::errors::Configuration)
	<< "no dictionary for class " << type.Name() << '\n';
    if(type.IsPointer()) type = type.ToType();

    pair<Member, bool> mem;

    // suitable members and number of integer->real casts required to get them
    vector<pair<int,Member> > oks;

    // first look in base scope
    for(Member_Iterator m = type.FunctionMember_Begin(); m != type.FunctionMember_End(); ++m ) {
      if(m->Name()==name) {
        int casts = checkMethod(*m, args, fixuppedArgs);
        if (casts > -1) {
            oks.push_back( make_pair(casts,*m) );
        } else {
           oError = -1*casts;
           //is this a show stopper error?
           if(fatalErrorCondition(oError)) {
              return mem;
           }
        }
      }
    }

    // found at least one method
    if (!oks.empty()) {
        if (oks.size() > 1) {
            // sort by number of conversiosns needed
            sort(oks.begin(), oks.end());

            if (oks[0].first == oks[1].first) { // two methods with same ambiguity
                throw edm::Exception(edm::errors::Configuration)
                    << "Can't resolve method " << name << " for " << type.Name() << ", the two candidates " 
                    << oks[0].second.Name() << " and " << oks[1].second.Name() 
                    << " require the same number of integer->real conversions (" << oks[0].first << ")\n";        
            }

            // I should fixup again the args, as both good methods have pushed them on fixuppedArgs
            fixuppedArgs.clear();
            checkMethod(oks.front().second, args, fixuppedArgs);
        }
        mem.first = oks.front().second;
    }

    // if nothing was found, look in parent scopes (without checking for cross-scope overloading, as it's not allowed)
    int baseError=parser::kNameDoesNotExist;
    if(! mem.first) {
      for(Base_Iterator b = type.Base_Begin(); b != type.Base_End(); ++ b) {
	      if((mem = findMethod(b->ToType(), name, args, fixuppedArgs,baseError)).first) break;
	      if(fatalErrorCondition(baseError)) {
            oError = baseError;
            return mem;
	      }
      }
    }


    // otherwise see if this object is just a Ref or Ptr and we should pop it out
    if(!mem.first) {
      // check for edm::Ref or edm::RefToBase or edm::Ptr
      if(type.IsTemplateInstance()) {
         TypeTemplate templ = type.TemplateFamily();
         std::string name = templ.Name();
         if(name.compare("Ref") == 0 ||
            name.compare("RefToBase") == 0 ||
            name.compare("Ptr") == 0) {
          // in this case  i think 'get' should be taken with no arguments!
          std::vector<AnyMethodArgument> empty, empty2; 
          int error;
          mem = findMethod(type, "get", empty, empty2,error);
          if(!mem.first) {
             throw edm::Exception(edm::errors::Configuration)
                << "no member \"get\" in reference of type " << type.Name() << "\n";        
          }
          mem.second = true;
         }
      }
    }
    /*
    if(!mem.first) {
      throw edm::Exception(edm::errors::Configuration)
	<< "member " << name << " not found in class "  << type.Name() << "\n";        
    }
    */
    if(mem.first) {
       oError = parser::kNoError;
    } else {
       //use error from base check if we never found function in primary class
       if(oError == parser::kNameDoesNotExist) {
          oError = baseError;
       }
    }
    return mem;
  }
}
