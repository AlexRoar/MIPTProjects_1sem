//
//  CommandsDTypes.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 13.10.2020.
//

#ifndef CommandsDTypes_h
#define CommandsDTypes_h


enum CommandParseResult {
    SPU_PARSE_OK,
    SPU_CMD_WRONG_ARGUMENTS,
    SPU_UNKNOWN_COMMAND,
    SPU_FINALPARSE_ERROR
};

#endif /* CommandsDTypes_h */
