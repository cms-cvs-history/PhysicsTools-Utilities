#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include "PhysicsTools/FWLite/src/AutoLibraryLoader.h"
using namespace std;

int main( int argc, char * argv[] ) {
  AutoLibraryLoader::enable();

  if ( argc != 2 ) {
    cerr << argv[0] << ":\n  usage: branchList fileName" << endl;
    return 1;
  }

  TFile file( argv[1] );
  TTree * events = dynamic_cast<TTree*>( file.Get( "Events" ) );
  assert( events != 0 );

  cout << "List of branches in file: " << argv[1] << endl;
  TObjArray * branches = events->GetListOfBranches();
  int n = branches->GetEntries();
  for( int i = 0; i < n; ++i ) {
    TBranch * branch = (TBranch*) branches->At( i );
    cout << i << ". " << branch->GetName() << endl;
  }

  return 0;
}
