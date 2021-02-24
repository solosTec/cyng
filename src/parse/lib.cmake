# 
#	reset 
#
set (parse_lib)

set (parse_cpp
    src/parse/hex.cpp
    src/parse/mac.cpp
    src/parse/version.cpp
    src/parse/duration.cpp
    src/parse/buffer.cpp
    src/parse/string.cpp
)
    
set (parse_h
    include/cyng/parse/hex.h
    include/cyng/parse/mac.h
    include/cyng/parse/version.h
    include/cyng/parse/duration.h
    include/cyng/parse/buffer.h
    include/cyng/parse/string.h
    include/cyng/parse/json.h
    include/cyng/parse/csv.h
)

set (parse_json
    include/cyng/parse/json/json_parser.h
    include/cyng/parse/json/json_symbol.h
    include/cyng/parse/json/json_tokenizer.h
    src/parse/json/json_parser.cpp
    src/parse/json/json_symbol.cpp
    src/parse/json/json_tokenizer.cpp
    src/parse/json/json.cpp
)

set (parse_csv
    include/cyng/parse/csv/csv_parser.h
    include/cyng/parse/csv/csv_symbol.h
    include/cyng/parse/csv/csv_tokenizer.h
    src/parse/csv/csv_parser.cpp
    src/parse/csv/csv_symbol.cpp
    src/parse/csv/csv_tokenizer.cpp
    src/parse/csv/csv.cpp
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

