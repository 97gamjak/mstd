for i in $(find test/ include/ scripts/mstd_cpp_checks -regex ".*\.\(hpp\|cpp\|c\|h\|py\)$"); do

    if ! grep -q Copyright $i; then

        echo "Adding license to $i"
        cat config/licenseHeader.txt $i >$i.new && mv $i.new $i

    fi

done
