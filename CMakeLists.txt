set(srsc)
set(includes)

list(APPEND includes
    # "code_timer/include"
    "circular_buffer/include"
    "sig_gen/include"
    "log_buffer/include"
)

list(APPEND srsc
    # "code_timer/code_timer.c"
    "circular_buffer/circular_buffer.c"
    "sig_gen/sig_gen.c"
    "sig_gen/sine.c"
    "log_buffer/log_buffer.c"
)


idf_component_register(SRCS ${srsc}
                    INCLUDE_DIRS ${includes}
                    )
