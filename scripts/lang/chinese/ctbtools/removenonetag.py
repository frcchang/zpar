import sys

if __name__ == "__main__":
   file = open(sys.argv[1])
   for line in file:
      line = line.strip().split()
      words = []
      for word in line:
         word = word.split("_")
         if word[0] == "":
            assert word[1] == "-NONE-"
         else:
            words.append("_".join(word))
      print " ".join(words)

   file.close()

