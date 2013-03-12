import sys
import re

g_reP = re.compile(r"^[,?!:;]$|^-LRB-$|^-RRB-$|^[.]+$|^[`]+$|^[']+$")

def evaluate(candidate, reference):
   fCandidate = open(candidate)
   fReference = open(reference)

   nTotal = 0
   nCorrect = 0
   nTotalSentences = 0

   nCorrectHeads = 0
   nCorrectDeps = 0
   nCorrectParentDeps = 0
   nTotalParentDeps = 0
   nCorrectNP = 0
   nTotalNP = 0

   sCandidate = fCandidate.readline()
   sReference = fReference.readline()
   nCandidateIndex = 0
   nReferenceIndex = 0
   dCandidateParents = {}
   dReferenceParents = {}
   lPunctuations = []
   lCandidateLink = []
   lReferenceLink = []

   nCorrectSent = 0
   nTotalSent = 0

   while sCandidate and sReference:

      sCandidate = sCandidate.strip()
      sReference = sReference.strip()

      if not sCandidate:
         for nIndex in dReferenceParents:
            if not nIndex in lPunctuations:
               nTotalParentDeps += 1
               if dCandidateParents.get(nIndex, []) == dReferenceParents[nIndex]:
                  nCorrectParentDeps += 1
         bCorrect = True
         for nIndex in xrange(len(lReferenceLink)):
            if not nIndex in lPunctuations:
               nTotalNP += 1
               if lReferenceLink[nIndex] == lCandidateLink[nIndex]:
                  nCorrectNP += 1
               else:
                  bCorrect = False
         nTotalSent += 1
         if bCorrect:
            nCorrectSent += 1
         sCandidate = fCandidate.readline()
         sReference = fReference.readline()	
         nCandidateIndex = 0
         nReferenceIndex = 0
         nTotalSentences += 1
         dCandidateParents = {}
         dReferenceParents = {}
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
      if not nCandidateParentIndex in dCandidateParents:
         dCandidateParents[nCandidateParentIndex] = []
      dCandidateParents[nCandidateParentIndex].append(nCandidateIndex)
      if not nReferenceParentIndex in dReferenceParents:
         dReferenceParents[nReferenceParentIndex] = []
      dReferenceParents[nReferenceParentIndex].append(nReferenceIndex)
#      if sReference.split()[1] in ["$", "``", "''", "-LRB-", "-RRB-", ",", ".", ":", "#"]:
      if g_reP.match( sReference.split()[0] ) :
         lPunctuations.append(nReferenceIndex)
      lReferenceLink.append(nReferenceParentIndex)
      lCandidateLink.append(nCandidateParentIndex)

      sCandidate = fCandidate.readline()
      sReference = fReference.readline()
      nCandidateIndex += 1
      nReferenceIndex += 1

   if sCandidate or sReference:
      print "Warning: candidate and reference different ln"

#   print "Dependency precision:", float(nCorrectDeps) / (nTotal-nTotalSentences), nCorrectDeps , nTotal-nTotalSentences
   print "Dependency precision without punctuations:", float(nCorrectNP) / (nTotalNP), nCorrectNP , nTotalNP
#   print "Parent precision:", float(nCorrectParentDeps) / (nTotalParentDeps), nCorrectParentDeps , nTotalParentDeps
#   print "Head precision:", float(nCorrectHeads) / nTotalSentences, nCorrectHeads, nTotalSentences
#   print "Word precision:", float(nCorrect) / nTotal, nCorrect, nTotal
   print "Sent precisions:", float(nCorrectSent) / nTotalSent

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print "The correct usage: evaldep.py output reference."
   evaluate(sys.argv[1], sys.argv[2])
