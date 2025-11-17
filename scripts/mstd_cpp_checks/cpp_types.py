from enum import Enum

class ResultTypeEnum(Enum):
    Ok = 0
    Warning = 1
    Error = 2
    
class ResultType:
    def __init__(self, value, description=None):
        self.value = value
        self.description = description

def check_key_sequence_ordered(key_sequence, line, filename, linenumber):
    
    if not isinstance(key_sequence, list):
        raise TypeError("key_sequence must be a list")
    
    indices = [line.split().index(key) for key in key_sequence if key in line]
    
    if len(indices) == len(key_sequence) and sorted(indices) != indices:
        return ResultType(ResultTypeEnum.Warning, f"key_sequence {key_sequence} not ordered correctly on line {line} in {filename}:{linenumber}")
        
    return ResultType(ResultTypeEnum.Ok)
        