#!/bin/bash
cat README.md | sed  -E 's/```([a-z]+)/```{.\1}/g' > README.docs.md
cmake --build _builds --target docs -- -j 8
./deploy.sh

