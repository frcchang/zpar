while read line; 
do
  echo "$line" | emacs -Q --batch --eval "(progn (insert \"$(cat)\") (goto-char 1) (replace-string \" (\" \"\n(\") (indent-region (point-min) (point-max) nil) (message (buffer-substring (point-min) (point-max))))" 2>&1 | grep -v '^[A-Z]'
done <$1
