#! /bin/bash
function compile_template {
	for i in $@; do
		echo "Compiling $i"
		cheetah fill $i -p > $(echo $i | sed "s/.tmpl//")
	done
}

compile_template $(find . -name *.tmpl)