#!/bin/sh

pathToRoot=`dirname $0`/..
for extension in cpp h h.in
do
    astyle --formatted --options=none --style=kr --indent=spaces=4 --align-pointer=name --align-reference=name --convert-tabs --attach-namespaces --max-code-length=100 --max-instatement-indent=120 --pad-header --pad-oper `find ${pathToRoot} -type f -name "*.${extension}"|grep -v ^${pathToRoot}/src/Box2D|grep -v ^${pathToRoot}/build`
done
