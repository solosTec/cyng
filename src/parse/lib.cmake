# 
#	reset 
#
set (parse_lib)

set (parse_cpp
    src/hex.cpp
    src/mac.cpp
    src/version.cpp
    src/duration.cpp
    src/timestamp.cpp
    src/color.cpp
    src/buffer.cpp
    src/string.cpp
    src/net.cpp
    src/raw.cpp
    src/op.cpp
)
    
set (parse_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/hex.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/mac.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/version.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/duration.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/timestamp.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/color.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/buffer.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/string.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/json.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/csv.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/net.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/raw.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/op.h
)

set (parse_json
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/json/json_parser.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/json/json_symbol.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/json/json_tokenizer.h
    src/json/json_parser.cpp
    src/json/json_symbol.cpp
    src/json/json_tokenizer.cpp
    src/json/json.cpp
)

set (parse_csv
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/csv/csv_parser.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/csv/csv_symbol.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/csv/csv_tokenizer.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/parse/csv/line_cast.hpp
    src/csv/csv_parser.cpp
    src/csv/csv_symbol.cpp
    src/csv/csv_tokenizer.cpp
    src/csv/csv.cpp
)

source_group("JSON" FILES ${parse_json})
source_group("CSV" FILES ${parse_csv})

# define the docscript lib
set (parse_lib
  ${parse_cpp}
  ${parse_h}
  ${parse_json}
  ${parse_csv}
)

