# --  PUBLIC FUNCTIONS  -- #

# Handles printing messages with a prefix.
function(sabre_message_status)
    __mono_message_impl(STATUS ${SABRE_TOOLCHAIN_TITLE} ${ARGN})
endfunction()
