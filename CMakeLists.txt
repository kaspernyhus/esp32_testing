set(srsc)
set(includes)

list(APPEND includes
    "code_timer/include"
    "ringbuffer/include"
    "sig_gen/include"
    "test_buffer/include"
)

list(APPEND srsc
    "code_timer/code_timer.c"
    "ringbuffer/ringbuffer.c"
    "sig_gen/sig_gen.c"
    "test_buffer/test_buffer.c"
)


idf_component_register(SRCS ${srsc}
                    INCLUDE_DIRS ${includes}
                    )
