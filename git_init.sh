#!/usr/bin/sh

branch="dev"
first_commit_msg="Initial commit"
remote="git@github.com:skalexey/abstract_ui.git"

echo "# abstract_ui" >> README.md

# don't add this script
gitignore_content=$(cat .gitignore)
echo "git_init.sh" >> .gitignore

git init
# git add *

# echo "${gitignore_content}" > .gitignore

# git add .gitignore

# git status

# git commit -m "${first_commit_msg}"
# git branch -M ${branch}
# git remote add origin ${remote}
# git push -u origin ${branch}