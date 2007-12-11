#!/bin/sh
md5sum -c md5sums
EXIT_CODE=${?}
exit ${EXIT_CODE}
