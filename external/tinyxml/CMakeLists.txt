add_library(tinyxml
        tinystr.cpp
        tinyxml.cpp
        tinyxmlerror.cpp
        tinyxmlparser.cpp)
set_property(TARGET tinyxml PROPERTY POSITION_INDEPENDENT_CODE ON)