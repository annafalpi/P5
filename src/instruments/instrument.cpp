#include <iostream>
#include "instrument_dumb.h"
#include "seno.h"
#include "instrument_FM.h"
#include "piano.h"
#include "guitar.h"
#include "bass.h"
#include "violin.h"

/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
			      const string &parameters) {
    Instrument * pInst = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "InstrumentDumb") {
      pInst = (Instrument *) new InstrumentDumb(parameters);
    }
    if (name == "seno") {
        pInst = (Instrument *) new seno(parameters);
      }
    if (name == "Fm") {
      pInst = (Instrument *) new instrumentFM(parameters);
    }
    if (name == "Piano") {
      pInst = (Instrument *) new piano(parameters);
    }
    if (name == "Guitar") {
      pInst = (Instrument *) new guitar(parameters);
    }
    if (name == "Bass") {
      pInst = (Instrument *) new bass(parameters);
    }
    if (name == "Violin") {
      pInst = (Instrument *) new violin(parameters);
    }
  return pInst;
  }
}
