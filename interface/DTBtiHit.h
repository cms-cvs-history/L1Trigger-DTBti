//-------------------------------------------------
//
/**  \class DTBtiHit
 *
 *   A class for hits in a drift cell
 *
 *
 *   $Date: 2004/07/09 17:14:41 $
 *   $Revision: 1.10 $
 *
 *   \author  C. Grandi, S. Vanini
 *   Modifications: 
 *   28/X/02 S.Vanini: tshift=425 included in register 
 *   1/IV/03 SV  time in clock units included: _clockTime
 *   --> operation with drift time are commented
 *   22/VI/04 SV: last trigger code update
 *
 */
//
//--------------------------------------------------
#ifndef DT_BTI_HIT_H
#define DT_BTI_HIT_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class DTDigi;

//----------------------
// Base Class Headers --
//----------------------
#include "L1Trigger/DTUtilities/interface/DTConfig.h"

//---------------
// C++ Headers --
//---------------
#include<cmath>
#include<string>


//              ---------------------
//              -- Class Interface --
//              ---------------------


class DTBtiHit {

  public:

  /// Constructor
  DTBtiHit(const DTDigi*, DTConfig*);

  /// Constructor from clock times
  DTBtiHit(int clockTime , DTConfig*);
		       
  /// Copy constructor
  DTBtiHit(const DTBtiHit&);
  
  /// Destructor 
  ~DTBtiHit();

  /// Assignment operator
  DTBtiHit & operator=(const DTBtiHit&);
  
  /// Move the hit forward in time one step
  inline void stepDownTime() { //_curTime-=_stepTime; 
                               _clockTime-=1; 
                              }

  /// Return the associated DTDigi
  inline const DTDigi* hitDigi() const { return _hitdigi; }

  /// Return the current time 
  /*! hits with curTime >0 correspond to avalanches drifting to wires
                        <0      "     "  signals already in the registers
  SV curTime=4000 if digis are given in clock units....
  */
  inline float curTime() const { return _curTime; }
  inline int clockTime() const { return _clockTime; }

  //! true if avalanche is still drifting
  inline int isDrifting() const { //return _curTime>=0 && _curTime<4000; 
                                  return _clockTime>1 && _clockTime<400; 
  }

  //! true if signal is in the registers
  //SV jtrig()=_config->lstep() added: is for tdrift==0
  inline int isInsideReg() const {
    //return _curTime<0 && jtrig()<=_config->lstep();
    return ( _clockTime<=0 && jtrig()<=_config->lstep() );      //SV bug fix 17XII03
  }

  //! position in registers
  inline int jtrig() const { 
                             //return (int)(fabs(_curTime)/_stepTime); 
                              return -_clockTime;
                            }
  //inline float jtrig() const { return fabs(_curTime)/_stepTime; } //prova SV
  //SV 13/XI/02 half-int simulation added
  /*inline float jtrig() const {  
        int idt = int(fabs(_curTime/_stepTime));
        float rest = fmod( fabs(_curTime), _stepTime );
        int irest = 0;
        if(rest==0.0)
          irest = 1;
        else
          irest = int( rest / (_stepTime*0.5) );
        float jtrig_int4 = float(idt) + float(irest)*0.5;
	return jtrig_int4;  
  }
*/

public:
  static const float _stepTime;
  static const float _stepTimeTdc;
  static std::string t0envFlag;

private:
  const DTDigi* _hitdigi;
  DTConfig* _config;
  float _curTime;
  int _clockTime;
};

#endif
