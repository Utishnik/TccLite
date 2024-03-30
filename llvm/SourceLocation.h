#include "ID_CORE.h"
#include <assert.h>

struct SourceLocation
{
private:
    ID_CORE ID;        

public:
    unsigned int MacroIDBit = 1ULL << (8 * sizeof(unsigned int) - 1);   
    bool isFileID() const  { return (ID.ID & MacroIDBit) == 0; }
    bool isMacroID() const { return (ID.ID & MacroIDBit) != 0; }
    /*
  static SourceLocation getFileLoc(unsigned int ID) {
    assert((ID & MacroIDBit) == 0 && "Ran out of source locations!");
    SourceLocation L;
    L.ID.ID = ID;
    return L;
  }

  static SourceLocation getMacroLoc(unsigned int ID) {
    assert((ID & MacroIDBit) == 0 && "Ran out of source locations!");
    SourceLocation L;
    L.ID.ID = MacroIDBit | ID;
    return L;
  }*/
  //todo fix err
};