def countlines(file):
   path = open(file)
   total = 0
   for line in path:
      total += 1
   close(path)
   return total
