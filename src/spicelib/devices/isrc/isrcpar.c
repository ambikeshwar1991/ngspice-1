/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: 2000 AlansFixes
**********/
/*
 */

#include "ngspice.h"
#include "ifsim.h"
#include "isrcdefs.h"
#include "sperror.h"
#include "suffix.h"
#include "1-f-code.h"


/* ARGSUSED */
int
ISRCparam(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    int i;	
    ISRCinstance *here = (ISRCinstance*)inst;

    NG_IGNORE(select);

    switch(param) {
        case ISRC_DC:
            here->ISRCdcValue = value->rValue;
            here->ISRCdcGiven = TRUE;
            break;
        case ISRC_AC_MAG:
            here->ISRCacMag = value->rValue;
            here->ISRCacMGiven = TRUE;
            here->ISRCacGiven = TRUE;
            break;
        case ISRC_AC_PHASE:
            here->ISRCacPhase = value->rValue;
            here->ISRCacPGiven = TRUE;
            here->ISRCacGiven = TRUE;
            break;
        case ISRC_AC:
            switch(value->v.numValue) {
                case 2:
                    here->ISRCacPhase = *(value->v.vec.rVec+1);
                    here->ISRCacPGiven = TRUE;
                case 1:
                    here->ISRCacMag = *(value->v.vec.rVec);
                    here->ISRCacMGiven = TRUE;
                case 0:
                    here->ISRCacGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case ISRC_PULSE:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = PULSE;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_SINE:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = SINE;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_EXP:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = EXP;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
        case ISRC_PWL:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = PWL;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            
            for (i=0;i<((here->ISRCfunctionOrder/2)-1);i++) {
                  if (*(here->ISRCcoeffs+2*(i+1))<=*(here->ISRCcoeffs+2*i)) {
                     fprintf(stderr, "Warning : current source %s",
                                                               here->ISRCname);
                     fprintf(stderr, " has non-increasing PWL time points.\n");
                  }
            }
            
            break;
        case ISRC_SFFM:
            if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = SFFM;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
            break;
	case ISRC_AM:
	    if(value->v.numValue <2) return(E_BADPARM);
            here->ISRCfunctionType = AM;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;
	    break;
	case ISRC_D_F1:
	    here->ISRCdF1given = TRUE;
	    here->ISRCdGiven = TRUE;
	    switch(value->v.numValue) {
		case 2:
		    here->ISRCdF1phase = *(value->v.vec.rVec+1);
		    here->ISRCdF1mag = *(value->v.vec.rVec);
		    break;
		case 1:
		    here->ISRCdF1mag = *(value->v.vec.rVec);
		    here->ISRCdF1phase = 0.0;
		    break;
		case 0:
		    here->ISRCdF1mag = 1.0;
		    here->ISRCdF1phase = 0.0;
		    break;
		default:
		    return(E_BADPARM);
	    }
	    break;
	case ISRC_D_F2:
	    here->ISRCdF2given = TRUE;
	    here->ISRCdGiven = TRUE;
	    switch(value->v.numValue) {
	        case 2:
		    here->ISRCdF2phase = *(value->v.vec.rVec+1);
		    here->ISRCdF2mag = *(value->v.vec.rVec);
		    break;
		case 1:
		    here->ISRCdF2mag = *(value->v.vec.rVec);
		    here->ISRCdF2phase = 0.0;
		    break;
		case 0:
		    here->ISRCdF2mag = 1.0;
		    here->ISRCdF2phase = 0.0;
		    break;
		default:
		    return(E_BADPARM);
	    }
	    break;

        case ISRC_TRNOISE: {
            double NA, TS;
            double NALPHA = 0.0;
            double NAMP   = 0.0;

            here->ISRCfunctionType = TRNOISE;
            here->ISRCfuncTGiven = TRUE;
            here->ISRCcoeffs = value->v.vec.rVec;
            here->ISRCfunctionOrder = value->v.numValue;
            here->ISRCcoeffsGiven = TRUE;

            NA = here->ISRCcoeffs[0]; // input is rms value
            TS = here->ISRCcoeffs[1]; // time step

            if (here->ISRCfunctionOrder > 2)
                NALPHA = here->ISRCcoeffs[2];

            if (here->ISRCfunctionOrder > 3 && NALPHA != 0.0)
                NAMP = here->ISRCcoeffs[3];

            here->ISRCtrnoise_state =
                trnoise_state_init(NA, TS, NALPHA, NAMP);
        }
            break;	

        default:
            return(E_BADPARM);
    }
    return(OK);
}
