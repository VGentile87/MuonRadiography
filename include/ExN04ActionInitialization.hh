#ifndef ExN04ActionInitialization_h
#define ExN04ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class ExN04DetectorConstruction;

/// Action initialization class.
///

class ExN04ActionInitialization : public G4VUserActionInitialization
{
  public:
    ExN04ActionInitialization(ExN04DetectorConstruction*);
    virtual ~ExN04ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    ExN04DetectorConstruction* fDetConstruction;
};

#endif

    
