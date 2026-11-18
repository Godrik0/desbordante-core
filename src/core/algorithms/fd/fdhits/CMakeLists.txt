set(NAME fd.fdhits)
desbordante_add_lib(NAME)
target_sources(
    ${NAME}
    PRIVATE fdhits.cpp
            pli_table.cpp
            treesearch/dynamic_edgemark_store.cpp
            treesearch/dynamic_edgemark_store_multi.cpp
            treesearch/treesearch_de.cpp
            treesearch/treesearch_multi.cpp
)
target_link_libraries(
    ${NAME}
    PRIVATE ${DESBORDANTE_PREFIX}::fd::hy::common
            ${DESBORDANTE_PREFIX}::fd::pli
            spdlog::spdlog_header_only
            Boost::headers
)
