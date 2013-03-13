# -encoding:utf8-
import sys
import re

g_reP = re.compile(r"^（$|^）$|^、$|^。$|^！$|^？$|^…$|^，$|^；$|^／$|^：$|^“$|^”$|^「$|^」$|^『$|^』$|^《$|^》$|^一一$")

def evaluate(candidate, reference):
   fCandidate = open(candidate)
   fReference = open(reference)

   nTotal = 0
   nCorrect = 0
   nTotalSentences = 0
   nCorrectSentence = 0

   nCorrectHeads = 0
   nCorrectDeps = 0
   nCorrectNP = 0
   nCorrectNPHead = 0
   nCorrectNPNon = 0
   nTotalNP = 0
   nTotalNPHead = 0
   nTotalHeads = 0

   sCandidate = fCandidate.readline()
   sReference = fReference.readline()
   nCandidateIndex = 0
   nReferenceIndex = 0
   lPunctuations = []
   lCandidateLink = []
   lReferenceLink = []

   while sCandidate and sReference:

      sCandidate = sCandidate.strip()
      sReference = sReference.strip()

      if not sCandidate:
         bCorrectSent = True
         for nIndex in xrange(len(lReferenceLink)):
            if not nIndex in lPunctuations:
               nTotalNP += 1
               if lReferenceLink[nIndex] == -1:
                  nTotalNPHead += 1
               if lReferenceLink[nIndex] == lCandidateLink[nIndex]:
                  nCorrectNP += 1
                  if lReferenceLink[nIndex] == -1:
                     nCorrectNPHead += 1
                  else:
                     nCorrectNPNon += 1
               else:
                  bCorrectSent = False
         assert lReferenceLink
         if bCorrectSent:
            nCorrectSentence += 1
         sCandidate = fCandidate.readline()
         sReference = fReference.readline()	
         nCandidateIndex = 0
         nReferenceIndex = 0
         nTotalSentences += 1
         lPunctuations = []
         lCandidateLink = []
         lReferenceLink = []
         continue
      else:
         nCandidateParentIndex = int(sCandidate.split()[2])
         nReferenceParentIndex = int(sReference.split()[2])

      nTotal += 1

      if nCandidateParentIndex == nReferenceParentIndex: 
         if nReferenceParentIndex == -1:
            nCorrectHeads += 1
         else:
            nCorrectDeps += 1
         nCorrect += 1
      if nReferenceParentIndex == -1:
         nTotalHeads += 1

      if sReference.split()[1] == "PU":
#      if g_reP.match( sReference.split()[0] ) :
         lPunctuations.append(nReferenceIndex)
      lReferenceLink.append(nReferenceParentIndex)
      lCandidateLink.append(nCandidateParentIndex)

      sCandidate = fCandidate.readline()
      sReference = fReference.readline()
      nCandidateIndex += 1
      nReferenceIndex += 1

   if sCandidate or sReference:
      print "Warning: candidate and reference different ln"

   assert nTotalHeads == nTotalSentences


   print "Dependency precision:", float(nCorrectDeps) / (nTotal-nTotalSentences), nCorrectDeps , nTotal-nTotalSentences
   print "Dependency precision without punctuations:", float(nCorrectNP) / (nTotalNP), nCorrectNP , nTotalNP
   print "Non-head precision:", float(nCorrectNP-nCorrectNPHead) / (nTotalNP-nTotalNPHead), nCorrectNP-nCorrectNPHead , nTotalNP-nTotalNPHead
   print "Head precision:", float(nCorrectNPHead) / nTotalNPHead, nCorrectNPHead, nTotalNPHead
   print "Sentence precision", float(nCorrectSentence) / nTotalSentences, nCorrectSentence, nTotalSentences

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print "The correct usage: evaldep.py output reference."
   evaluate(sys.argv[1], sys.argv[2])
