//
//  CommandsDTypes.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 13.10.2020.
//

#ifndef CommandsDTypes_h
#define CommandsDTypes_h


enum CommandParseResult {
    SPU_PARSE_OK             = 600,
    SPU_CMD_WRONG_ARGUMENTS  = 601,
    SPU_UNKNOWN_COMMAND      = 602,
    SPU_FINALPARSE_ERROR     = 603
};

#endif /* CommandsDTypes_h */
