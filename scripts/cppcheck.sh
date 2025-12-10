cppcheck --enable=all \
    --enable=performance \
    --enable=style \
    --enable=information \
    --enable=portability \
    --error-exitcode=1 \
    --suppressions-list=.cppcheck.suppress \
    --suppress=missingIncludeSystem \
    --inline-suppr \
    --inconclusive \
    -I include \
    include test
