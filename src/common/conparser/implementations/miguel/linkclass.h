
#ifndef _COMMON_CON_PARSER_LINK
#define _COMMON_CON_PARSER_LINK

//class CStateNode;

/*===============================================================
 *
 * CLINK - stanford dependencies in ZPar (links) 
 * Authors: Miguel Ballesteros and Yue Zhang
 * UPF-Spain and SUTD-Singapore
 *
 *==============================================================*/

class CLink {
public:
   CDependencyLabel label; //src/english/dependency/label/stanford.h
   int dependent;
   int head; 
   CLink *next;   

public:
   CLink(const CDependencyLabel label, const int dependent, const int head, CLink* next): label(label), dependent(dependent), head(head), next(next) {}   
   CLink(): label(0), dependent(-1), head(-1), next(0) {}	
   
   virtual ~CLink() {}

public:
   bool valid() const { return dependent!=-1 && head!=-1 && label!=0; }
   void clear() { 
      this->label = CDependencyLabel::NONE;
      this->dependent=-1;
      this->next=0;
      this->head=-1;
   }
   void set(const CDependencyLabel label, const int dependent, CLink* next, const int head) { 
      this->label = label;
      this->dependent = dependent; 
      this->next = next;
      this->head=head;
   }
   
   
   bool operator == (const CLink &nd) const {
      return dependent == nd.dependent &&
             label == nd.label && 
             head == nd.head &&
             next == nd.next;
   }
   void operator = (const CLink &nd) {
	   dependent = nd.dependent;
       label = nd.label; 
       next = nd.next;   
       head = nd.head;
   }
    

/*public:
   void toCCFGTreeNode(CCFGTreeNode &node) const {
//      node.parent = parent->id;
      node.is_constituent = is_constituent();
      node.temp = temp;
#ifdef NO_TEMP_CONSTITUENT
      ASSERT(!node.temp, "Internal error: this version does not temporary constituents but state items have them.")
      node.constituent = constituent.code();
#else
      node.constituent = constituent.extractConstituentCode();
#endif
      node.single_child = single_child();
      node.head_left = head_left();
      node.left_child = left_child ? left_child->id : -1;
      node.right_child = right_child ? right_child->id : -1;
      node.token = lexical_head;
   }
};*/



};





/*===============================================================
 * CHeadFinder 
 * Implements a 'semantic head' variant of the the HeadFinder found
 * in Michael Collins' 1999 thesis.
 *  See SemanticHeadFinder in Stanford parser.
 ===============================================================*/
 
 class CHeadFinder {
	 
	 /*
	  * 
	  *  private static final String[] auxiliaries = {"will", "wo", "shall", "sha", "may", "might", "should", "would", "can", "could", "ca", "must", "has", "have", "had", "having", "get", "gets", "getting", "got", "gotten", "do", "does", "did", "to", "'ve", "ve", "'d", "d", "'ll", "ll", "na" };
  private static final String[] beGetVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "get", "getting", "gets", "got"};
  private static final String[] copulaVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"};

  private static final String[] verbTags = {"TO", "MD", "VB", "VBD", "VBP", "VBZ", "VBG", "VBN", "AUX", "AUXG"};
	  */
 public:
	 
	 //collinsheadfinder and abstractcollinsheadfinder and ModCollinsHeadFinder
	 std::map<std::string, std::string**> nonTerminalInfo;
	 
	 
	 //SemanticHeadFinder
	 std::string verbTags[10];
	 std::string auxiliaries[32];
	 std::string copulateVerbs[32];
	 std::string beGetVerbs[18];
	 
	 /*
	  *private static final String[] auxiliaries = {"will", "wo", "shall", "sha", "may", "might", "should", "would", "can", "could", "ca", "must", "has", "have", "had", "having", "get", "gets", "getting", "got", "gotten", "do", "does", "did", "to", "'ve", "ve", "'d", "d", "'ll", "ll", "na" };
	   private static final String[] beGetVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "get", "getting", "gets", "got"};
	   private static final String[] copulaVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"}
	   private static final String[] verbTags = {"TO", "MD", "VB", "VBD", "VBP", "VBZ", "VBG", "VBN", "AUX", "AUXG"};
	  */
	 
 	public:
 		int id;
 		
 		CHeadFinder() : verbTags({"TO", "MD", "VB", "VBD", "VBP", "VBZ", "VBG", "VBN", "AUX", "AUXG"}), 
 					auxiliaries({"will", "wo", "shall", "sha", "may", "might", "should", "would", "can", "could", "ca", "must", "has", "have", "had", "having", "get", "gets", "getting", "got", "gotten", "do", "does", "did", "to", "'ve", "ve", "'d", "d", "'ll", "ll", "na" }),
 					copulateVerbs({"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"}),
 					beGetVerbs({"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "get", "getting", "gets", "got"})
 		{
 			
 			//MIGUEL: there is no implementation of the Constructor yet. THERE MUST BE.
 			
 			//This is from ModCollinsHeadFinder (Stanford Parser code)
 			//nonTerminalInfo.put("ADJP", ....hecho {"rightdis", "JJP", "JJR", "JJS", "DT", "RB", "RBR", "CD", "IN", "VBD"}, {"left", "ADVP", "NP"}});
 			nonTerminalInfo["ADJP"]=new std::string*[5];
 			nonTerminalInfo["ADJP"][0]=new std::string[2];
 			nonTerminalInfo["ADJP"][0][0]="left";
 			nonTerminalInfo["ADJP"][0][1]="$";
 			nonTerminalInfo["ADJP"][1]=new std::string[7];
 			nonTerminalInfo["ADJP"][1][0]="rightdis";
 			nonTerminalInfo["ADJP"][1][1]="NNS";
 			nonTerminalInfo["ADJP"][1][2]="NN";
 			nonTerminalInfo["ADJP"][1][3]="JJ";
 			nonTerminalInfo["ADJP"][1][4]="QP";
 			nonTerminalInfo["ADJP"][1][5]="VBN";
 			nonTerminalInfo["ADJP"][1][6]="VBG";
 			nonTerminalInfo["ADJP"][2]=new std::string[2];
 			nonTerminalInfo["ADJP"][2][0]="left";
 			nonTerminalInfo["ADJP"][2][1]="ADJP";
 			nonTerminalInfo["ADJP"][3]=new std::string[10];
 			nonTerminalInfo["ADJP"][3][0]="rightdis";
 			nonTerminalInfo["ADJP"][3][1]="JJP";
 			nonTerminalInfo["ADJP"][3][2]="JJR";
 			nonTerminalInfo["ADJP"][3][3]="JJS";
 			nonTerminalInfo["ADJP"][3][4]="DT";
 			nonTerminalInfo["ADJP"][3][5]="RB";
 			nonTerminalInfo["ADJP"][3][6]="RBR";
 			nonTerminalInfo["ADJP"][3][7]="CD";
 			nonTerminalInfo["ADJP"][3][8]="IN";
 			nonTerminalInfo["ADJP"][3][9]="VBD";
 			nonTerminalInfo["ADJP"][4]=new std::string[3];
 			nonTerminalInfo["ADJP"][4][0]="left";
 			nonTerminalInfo["ADJP"][4][1]="ADVP";			
 			nonTerminalInfo["ADJP"][4][1]="NP";
 			
 			//nonTerminalInfo.put("JJP", new String[][]{{"left", "NNS", "NN", "$", "QP", "JJ", "VBN", "VBG", "ADJP", "JJP", "JJR", "NP", "JJS", "DT", "FW", "RBR", "RBS", "SBAR", "RB"}});  // JJP is introduced for NML-like adjective phrases in Vadas' treebank; Chris wishes he hadn't used JJP which should be a POS-tag. 			
 			//The people from stanford think that perhaps this should not be included.
 			nonTerminalInfo["JJP"]=new std::string*[1];
 			nonTerminalInfo["JJP"][0]=new std::string[19];
 			nonTerminalInfo["JJP"][0][0]="left";
 			nonTerminalInfo["JJP"][0][1]="NNS";
 			nonTerminalInfo["JJP"][0][2]="NN";
 			nonTerminalInfo["JJP"][0][3]="$";
 			nonTerminalInfo["JJP"][0][4]="QP";
 			nonTerminalInfo["JJP"][0][5]="JJ";
 			nonTerminalInfo["JJP"][0][6]="VBN";
 			nonTerminalInfo["JJP"][0][7]="VBG";
 			nonTerminalInfo["JJP"][0][8]="ADJP";
 			nonTerminalInfo["JJP"][0][9]="JJP";
 			nonTerminalInfo["JJP"][0][10]="JJR";
 			nonTerminalInfo["JJP"][0][11]="NP";
 			nonTerminalInfo["JJP"][0][12]="JJS";
 			nonTerminalInfo["JJP"][0][13]="DT";
 			nonTerminalInfo["JJP"][0][14]="FW";
 			nonTerminalInfo["JJP"][0][15]="RBR";
 			nonTerminalInfo["JJP"][0][16]="RBS";
 			nonTerminalInfo["JJP"][0][17]="SBAR";
 			nonTerminalInfo["JJP"][0][18]="RB";
 			
 			//// ADVP rule rewritten by Chris in Nov 2010 to be rightdis.  This is right! JJ.* is often head and rightmost. //Comment in STF code.
 			//nonTerminalInfo.put("ADVP", new String[][]{{"left", "ADVP", "IN"},
 			// 			        {"rightdis", "RB", "RBR", "RBS", "JJ", "JJR", "JJS"},
 			// 			        {"rightdis", "RP", "DT", "NN", "CD", "NP", "VBN", "NNP", "CC", "FW", "NNS", "ADJP", "NML"}});
 			
 			nonTerminalInfo["ADVP"]=new std::string*[3];
 			nonTerminalInfo["ADVP"][0]=new std::string[3];
 			nonTerminalInfo["ADVP"][0][0]="left";
 			nonTerminalInfo["ADVP"][0][1]="ADVP";
 			nonTerminalInfo["ADVP"][0][2]="IN";
 			nonTerminalInfo["ADVP"][1]=new std::string[7];
 			nonTerminalInfo["ADVP"][1][0]="rightdis";
 			nonTerminalInfo["ADVP"][1][1]="RB";
 			nonTerminalInfo["ADVP"][1][2]="RBR";
 			nonTerminalInfo["ADVP"][1][3]="RBS";
 			nonTerminalInfo["ADVP"][1][4]="JJ";
 			nonTerminalInfo["ADVP"][1][5]="JJR";
 			nonTerminalInfo["ADVP"][1][6]="JJS";
 			nonTerminalInfo["ADVP"][2]=new std::string[13];
 			nonTerminalInfo["ADVP"][2][0]="rightdis";
 			nonTerminalInfo["ADVP"][2][1]="RP";
 			nonTerminalInfo["ADVP"][2][2]="DT";
 			nonTerminalInfo["ADVP"][2][3]="NN";
 			nonTerminalInfo["ADVP"][2][4]="CD";
 			nonTerminalInfo["ADVP"][2][5]="NP";
 			nonTerminalInfo["ADVP"][2][6]="VBN";
 			nonTerminalInfo["ADVP"][2][7]="NNP";
 			nonTerminalInfo["ADVP"][2][8]="CC";
 			nonTerminalInfo["ADVP"][2][9]="FW";
 			nonTerminalInfo["ADVP"][2][10]="NNS";
 			nonTerminalInfo["ADVP"][2][11]="ADJP";
 			nonTerminalInfo["ADVP"][2][12]="NML";
 			
 			//nonTerminalInfo.put("CONJP", new String[][]{{"right", "CC", "RB", "IN"}});
 			
 			/*nonTerminalInfo["CONJP"]=new std::string*[1];
 			nonTerminalInfo["CONJP"][0]=new std::string[4];
 			nonTerminalInfo["CONJP"][0][0]="right";
 			nonTerminalInfo["CONJP"][0][1]="CC";
 			nonTerminalInfo["CONJP"][0][2]="RB";
 			nonTerminalInfo["CONJP"][0][3]="IN";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["CONJP"]=new std::string*[1];
 			 			nonTerminalInfo["CONJP"][0]=new std::string[6];
 			 			nonTerminalInfo["CONJP"][0][0]="right";
 			 			nonTerminalInfo["CONJP"][0][1]="VB";
 			 			nonTerminalInfo["CONJP"][0][2]="JJ";
 			 			nonTerminalInfo["CONJP"][0][3]="RB";
 			 			nonTerminalInfo["CONJP"][0][4]="IN";
 			 			nonTerminalInfo["CONJP"][0][5]="CC";
 			
 			
 			//nonTerminalInfo.put("FRAG", new String[][]{{"right"}}); // crap
 			/*nonTerminalInfo["FRAG"]=new std::string*[1];
 			nonTerminalInfo["FRAG"][0]=new std::string[1];
 			nonTerminalInfo["FRAG"][0][0]="right";*/
 			 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			 			nonTerminalInfo["FRAG"]=new std::string*[4];
 			 			nonTerminalInfo["FRAG"][0]=new std::string[2];
 			 			nonTerminalInfo["FRAG"][0][0]="left";
 			 			nonTerminalInfo["FRAG"][0][1]="IN";
 			 			nonTerminalInfo["FRAG"][1]=new std::string[2];
 			 			nonTerminalInfo["FRAG"][1][0]="right";
 			 			nonTerminalInfo["FRAG"][1][1]="RB";
 			 			nonTerminalInfo["FRAG"][2]=new std::string[2];
 			 			nonTerminalInfo["FRAG"][2][0]="left";
 			 			nonTerminalInfo["FRAG"][2][1]="NP";
 			 			nonTerminalInfo["FRAG"][3]=new std::string[7];
 			 			nonTerminalInfo["FRAG"][3][0]="left";
 			 			nonTerminalInfo["FRAG"][3][1]="ADJP";
 			 			nonTerminalInfo["FRAG"][3][2]="ADVP";
 			 			nonTerminalInfo["FRAG"][3][3]="FRAG"; 			 			
 			 			nonTerminalInfo["FRAG"][3][4]="S";
 			 			nonTerminalInfo["FRAG"][3][5]="SBAR";
 			 			nonTerminalInfo["FRAG"][3][6]="VP";
 			 			
 			
			//nonTerminalInfo.put("INTJ", new String[][]{{"left"}});
 			nonTerminalInfo["INTJ"]=new std::string*[1];
 			nonTerminalInfo["INTJ"][0]=new std::string[1];
 			nonTerminalInfo["INTJ"][0][0]="left";
 			
 			//nonTerminalInfo.put("LST", new String[][]{{"right", "LS", ":"}});
 			nonTerminalInfo["LST"]=new std::string*[1];
 			nonTerminalInfo["LST"][0]=new std::string[3];
 			nonTerminalInfo["LST"][0][0]="right";
 			nonTerminalInfo["LST"][0][1]="LS";
 			nonTerminalInfo["LST"][0][2]=":";

 			// NML is head in: (NAC-LOC (NML San Antonio) (, ,) (NNP Texas))
 			// TODO: NNP should be head (rare cases, could be ignored):
 			//   (NAC (NML New York) (NNP Court) (PP of Appeals))
 			//   (NAC (NML Prudential Insurance) (NNP Co.) (PP Of America))
 			// Chris: This could maybe still do with more thought, but NAC is rare.
 			//nonTerminalInfo.put("NAC", new String[][]{{"left", "NN", "NNS", "NML", "NNP", "NNPS", "NP", "NAC", "EX", "$", "CD", "QP", "PRP", "VBG", "JJ", "JJS", "JJR", "ADJP", "JJP", "FW"}});
 			nonTerminalInfo["NAC"]=new std::string*[1];
 			nonTerminalInfo["NAC"][0]=new std::string[2];
 			nonTerminalInfo["NAC"][0][0]="left";
 			nonTerminalInfo["NAC"][0][1]="NN";
 			nonTerminalInfo["NAC"][0][2]="NNS";
 			nonTerminalInfo["NAC"][0][3]="NML";
 			nonTerminalInfo["NAC"][0][4]="NNP";
 			nonTerminalInfo["NAC"][0][5]="NNPS";
 			nonTerminalInfo["NAC"][0][6]="NP";
 			nonTerminalInfo["NAC"][0][7]="NAC";
 			nonTerminalInfo["NAC"][0][8]="EX";
 			nonTerminalInfo["NAC"][0][9]="$";
 			nonTerminalInfo["NAC"][0][10]="CD";
 			nonTerminalInfo["NAC"][0][11]="QP";
 			nonTerminalInfo["NAC"][0][12]="PRP";
 			nonTerminalInfo["NAC"][0][13]="VBG";
 			nonTerminalInfo["NAC"][0][14]="JJ";
 			nonTerminalInfo["NAC"][0][15]="JJS";
 			nonTerminalInfo["NAC"][0][16]="JJR";
 			nonTerminalInfo["NAC"][0][17]="ADJP";
 			nonTerminalInfo["NAC"][0][18]="JJP";
 			nonTerminalInfo["NAC"][0][19]="FW";
 			
			//nonTerminalInfo.put("NX", new String[][]{{"right", "NP", "NX"}});
 			nonTerminalInfo["NX"]=new std::string*[1];
 			nonTerminalInfo["NX"][0]=new std::string[3];
 			nonTerminalInfo["NX"][0][0]="right";
 			nonTerminalInfo["NX"][0][1]="NP";
 			nonTerminalInfo["NX"][0][2]="NX";
 			
 			//nonTerminalInfo.put("PP", new String[][]{{"right", "IN", "TO", "VBG", "VBN", "RP", "FW", "JJ", "SYM"}, {"left", "PP"}});
 			
 			nonTerminalInfo["PP"]=new std::string*[2];
 			nonTerminalInfo["PP"][0]=new std::string[9];
 			nonTerminalInfo["PP"][0][0]="right";
 			nonTerminalInfo["PP"][0][1]="IN";
 			nonTerminalInfo["PP"][0][2]="TO";
 			nonTerminalInfo["PP"][0][3]="VBG";
 			nonTerminalInfo["PP"][0][4]="VBN";
 			nonTerminalInfo["PP"][0][5]="RP";
 			nonTerminalInfo["PP"][0][6]="FW";
 			nonTerminalInfo["PP"][0][7]="JJ";
 			nonTerminalInfo["PP"][0][8]="IN";
 			nonTerminalInfo["PP"][1]=new std::string[2];
 			nonTerminalInfo["PP"][1][0]="left";
 			nonTerminalInfo["PP"][1][1]="PP";
 			
 			//nonTerminalInfo.put("PRN", new String[][]{{"left", "VP", "NP", "PP", "SQ", "S", "SINV", "SBAR", "ADJP", "JJP", "ADVP", "INTJ", "WHNP", "NAC", "VBP", "JJ", "NN", "NNP"}});
 			
 			/*nonTerminalInfo["PRN"]=new std::string*[1];
 			nonTerminalInfo["PRN"][0]=new std::string[18];
 			nonTerminalInfo["PRN"][0][0]="left";
 			nonTerminalInfo["PRN"][0][1]="VP";
 			nonTerminalInfo["PRN"][0][2]="NP";
 			nonTerminalInfo["PRN"][0][3]="PP";
 			nonTerminalInfo["PRN"][0][4]="SQ";
 			nonTerminalInfo["PRN"][0][5]="S";
 			nonTerminalInfo["PRN"][0][6]="SINV";
 			nonTerminalInfo["PRN"][0][7]="SBAR";
 			nonTerminalInfo["PRN"][0][8]="ADJP";
 			nonTerminalInfo["PRN"][0][9]="JJP";
 			nonTerminalInfo["PRN"][0][10]="ADVP";
 			nonTerminalInfo["PRN"][0][11]="INTJ";
 			nonTerminalInfo["PRN"][0][12]="WHNP";
 			nonTerminalInfo["PRN"][0][13]="NAC";
 			nonTerminalInfo["PRN"][0][14]="VBP";
 			nonTerminalInfo["PRN"][0][15]="JJ";
 			nonTerminalInfo["PRN"][0][16]="NN";
 			nonTerminalInfo["PRN"][0][17]="NNP";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			nonTerminalInfo["PRN"]=new std::string*[1];
 			nonTerminalInfo["PRN"][0]=new std::string[17];
 			nonTerminalInfo["PRN"][0][0]="left";
 			nonTerminalInfo["PRN"][0][1]="VP";
 			nonTerminalInfo["PRN"][0][2]="SQ";
 			nonTerminalInfo["PRN"][0][3]="S";
 			nonTerminalInfo["PRN"][0][4]="SINV";
 			nonTerminalInfo["PRN"][0][5]="SBAR";
 			nonTerminalInfo["PRN"][0][6]="NP";
 			nonTerminalInfo["PRN"][0][7]="ADJP";
 			nonTerminalInfo["PRN"][0][8]="PP";
 			nonTerminalInfo["PRN"][0][9]="ADVP";
 			nonTerminalInfo["PRN"][0][10]="INTJ";
 			nonTerminalInfo["PRN"][0][11]="WHNP";
 			nonTerminalInfo["PRN"][0][12]="NAC";
 			nonTerminalInfo["PRN"][0][13]="VBP";
 			nonTerminalInfo["PRN"][0][14]="JJ";
 			nonTerminalInfo["PRN"][0][15]="NN";
 			nonTerminalInfo["PRN"][0][16]="NNP";
 			
 			//nonTerminalInfo.put("PRT", new String[][]{{"right", "RP"}});
 			nonTerminalInfo["PRT"]=new std::string*[1];
 			nonTerminalInfo["PRT"][0]=new std::string[2];
 			nonTerminalInfo["PRT"][0][0]="right";
 			nonTerminalInfo["PRT"][0][1]="RP";
 			
 			// add '#' for pounds!! (this comment in stf code ???)
 			//nonTerminalInfo.put("QP", new String[][]{{"left", "$", "IN", "NNS", "NN", "JJ", "CD", "PDT", "DT", "RB", "NCD", "QP", "JJR", "JJS"}});
 			/*nonTerminalInfo["QP"]=new std::string*[1];
 			nonTerminalInfo["QP"][0]=new std::string[14];
 			nonTerminalInfo["QP"][0][0]="left";
 			nonTerminalInfo["QP"][0][1]="$";
 			nonTerminalInfo["QP"][0][2]="IN";
 			nonTerminalInfo["QP"][0][3]="NNS";
 			nonTerminalInfo["QP"][0][4]="NN";
 			nonTerminalInfo["QP"][0][5]="JJ";
 			nonTerminalInfo["QP"][0][6]="CD";
 			nonTerminalInfo["QP"][0][7]="PDT";
 			nonTerminalInfo["QP"][0][8]="DT";
 			nonTerminalInfo["QP"][0][9]="RB";
 			nonTerminalInfo["QP"][0][10]="NCD";
 			nonTerminalInfo["QP"][0][11]="QP";
 			nonTerminalInfo["QP"][0][12]="JJR";
 			nonTerminalInfo["QP"][0][13]="JJS";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["QP"]=new std::string*[1];
 			 			nonTerminalInfo["QP"][0]=new std::string[14];
 			 			nonTerminalInfo["QP"][0][0]="right";
 			 			nonTerminalInfo["QP"][0][1]="$";
 			 			nonTerminalInfo["QP"][0][2]="NNS";
 			 			nonTerminalInfo["QP"][0][3]="NN";
 			 			nonTerminalInfo["QP"][0][4]="CD";
 			 			nonTerminalInfo["QP"][0][5]="JJ";
 			 			nonTerminalInfo["QP"][0][6]="PDT";
 			 			nonTerminalInfo["QP"][0][7]="DT";
 			 			nonTerminalInfo["QP"][0][8]="IN";
 			 			nonTerminalInfo["QP"][0][9]="RB";
 			 			nonTerminalInfo["QP"][0][10]="NCD";
 			 			nonTerminalInfo["QP"][0][11]="QP";
 			 			nonTerminalInfo["QP"][0][12]="JJR";
 			 			nonTerminalInfo["QP"][0][13]="JJS";
 			
 			//nonTerminalInfo.put("RRC", new String[][]{{"right", "VP", "NP", "ADVP", "ADJP", "JJP", "PP"}});
 			nonTerminalInfo["RRC"]=new std::string*[1];
 			nonTerminalInfo["RRC"][0]=new std::string[7];
 			nonTerminalInfo["RRC"][0][0]="right";
 			nonTerminalInfo["RRC"][0][1]="VP";
 			nonTerminalInfo["RRC"][0][2]="NP";
 			nonTerminalInfo["RRC"][0][3]="ADVP";
 			nonTerminalInfo["RRC"][0][4]="ADJP";
 			nonTerminalInfo["RRC"][0][5]="JJP";
 			nonTerminalInfo["RRC"][0][6]="PP";
 			
 			 // delete IN -- go for main part of sentence; add FRAG ...this comment in stf code ??
 			
 			//nonTerminalInfo.put("S", new String[][]{{"left", "TO", "VP", "S", "FRAG", "SBAR", "ADJP", "JJP", "UCP", "NP"}});
 			/*nonTerminalInfo["S"]=new std::string*[1];
 			nonTerminalInfo["S"][0]=new std::string[10];
 			nonTerminalInfo["S"][0][0]="left";
 			nonTerminalInfo["S"][0][1]="TO";
 			nonTerminalInfo["S"][0][2]="VP";
 			nonTerminalInfo["S"][0][3]="S";
 			nonTerminalInfo["S"][0][4]="FRAG";
 			nonTerminalInfo["S"][0][5]="SBAR";
 			nonTerminalInfo["S"][0][6]="ADJP";
 			nonTerminalInfo["S"][0][7]="JJP";
 			nonTerminalInfo["S"][0][8]="UCP";
 			nonTerminalInfo["S"][0][9]="NP";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			nonTerminalInfo["S"]=new std::string*[2];
 			nonTerminalInfo["S"][0]=new std::string[7];
 			nonTerminalInfo["S"][0][0]="left";
 			nonTerminalInfo["S"][0][1]="VP";
 			nonTerminalInfo["S"][0][1]="S";
 			nonTerminalInfo["S"][0][2]="FRAG";
 			nonTerminalInfo["S"][0][3]="SBAR";
 			nonTerminalInfo["S"][0][4]="ADJP";
 			nonTerminalInfo["S"][0][5]="UCP";
 			nonTerminalInfo["S"][0][6]="TO";
 			nonTerminalInfo["S"][0]=new std::string[2];
 			nonTerminalInfo["S"][1][0]="right";
 			nonTerminalInfo["S"][1][1]="NP";
 			
 			
 		   //nonTerminalInfo.put("SBAR", new String[][]{{"left", "WHNP", "WHPP", "WHADVP", "WHADJP", "IN", "DT", "S", "SQ", "SINV", "SBAR", "FRAG"}});
 			/*nonTerminalInfo["SBAR"]=new std::string*[1];
 			nonTerminalInfo["SBAR"][0]=new std::string[12];
 			nonTerminalInfo["SBAR"][0][0]="left";
 			nonTerminalInfo["SBAR"][0][1]="WHNP";
 			nonTerminalInfo["SBAR"][0][2]="WHPP";
 			nonTerminalInfo["SBAR"][0][3]="WHADVP";
 			nonTerminalInfo["SBAR"][0][4]="WHADJP";
 			nonTerminalInfo["SBAR"][0][5]="IN";
 			nonTerminalInfo["SBAR"][0][6]="DT";
 			nonTerminalInfo["SBAR"][0][7]="S";
 			nonTerminalInfo["SBAR"][0][8]="SQ";
 			nonTerminalInfo["SBAR"][0][9]="SINV";
 			nonTerminalInfo["SBAR"][0][10]="SBAR";
 			nonTerminalInfo["SBAR"][0][11]="FRAG";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["SBAR"]=new std::string*[1];
 			 			nonTerminalInfo["SBAR"][0]=new std::string[13];
 			 			nonTerminalInfo["SBAR"][0][0]="left";
 			 			nonTerminalInfo["SBAR"][0][1]="S";
 			 			nonTerminalInfo["SBAR"][0][2]="SQ";
 			 			nonTerminalInfo["SBAR"][0][3]="SINV";
 			 			nonTerminalInfo["SBAR"][0][4]="SBAR";
 			 			nonTerminalInfo["SBAR"][0][5]="FRAG";
 			 			nonTerminalInfo["SBAR"][0][6]="VP";
 			 			nonTerminalInfo["SBAR"][0][7]="WHNP";
 			 			nonTerminalInfo["SBAR"][0][8]="WHPP";
 			 			nonTerminalInfo["SBAR"][0][9]="WHADVP";
 			 			nonTerminalInfo["SBAR"][0][10]="WHADJP";
 			 			nonTerminalInfo["SBAR"][0][11]="IN";
 			 			nonTerminalInfo["SBAR"][0][12]="DT";
 			
 			
 			// nonTerminalInfo.put("SBARQ", new String[][]{{"left", "SQ", "S", "SINV", "SBARQ", "FRAG", "SBAR"}});
 			nonTerminalInfo["SBARQ"]=new std::string*[1];
 			nonTerminalInfo["SBARQ"][0]=new std::string[7];
 			nonTerminalInfo["SBARQ"][0][0]="left";
 			nonTerminalInfo["SBARQ"][0][1]="SQ";
 			nonTerminalInfo["SBARQ"][0][2]="S";
 			nonTerminalInfo["SBARQ"][0][3]="SINV";
 			nonTerminalInfo["SBARQ"][0][4]="SBARQ";
 			nonTerminalInfo["SBARQ"][0][5]="FRAG";
 			nonTerminalInfo["SBARQ"][0][6]="SBAR";

 			//This comment in stf code. cdm: if you have 2 VP under an SINV, you should really take the 2nd as syntactic head, because the first is a topicalized VP complement of the second, but for now I didn't change this, since it didn't help parsing.  (If it were changed, it'd need to be also changed to the opposite in SemanticHeadFinder.)
 			//nonTerminalInfo.put("SINV", new String[][]{{"left", "VBZ", "VBD", "VBP", "VB", "MD", "VBN", "VP", "S", "SINV", "ADJP", "JJP", "NP"}});
 			nonTerminalInfo["SINV"]=new std::string*[1];
 			nonTerminalInfo["SINV"][0]=new std::string[12];
 			nonTerminalInfo["SINV"][0][0]="left";
 			nonTerminalInfo["SINV"][0][1]="VBZ";
 			nonTerminalInfo["SINV"][0][2]="VBD";
 			nonTerminalInfo["SINV"][0][3]="VBP";
 			nonTerminalInfo["SINV"][0][4]="VB";
 			nonTerminalInfo["SINV"][0][5]="MD";
 			nonTerminalInfo["SINV"][0][6]="VP";
 			nonTerminalInfo["SINV"][0][7]="S";
 			nonTerminalInfo["SINV"][0][8]="SINV";
 			nonTerminalInfo["SINV"][0][9]="ADJP";
 			nonTerminalInfo["SINV"][0][10]="JJP";
 			nonTerminalInfo["SINV"][0][11]="NP";
 			
 			// nonTerminalInfo.put("SQ", new String[][]{{"left", "VBZ", "VBD", "VBP", "VB", "MD", "AUX", "AUXG", "VP", "SQ"}});  // TODO: Should maybe put S before SQ for tag questions. Check. This comment in stf dependencies.
 			/*nonTerminalInfo["SQ"]=new std::string*[1];
 			nonTerminalInfo["SQ"][0]=new std::string[10];
 			nonTerminalInfo["SQ"][0][0]="left";
 			nonTerminalInfo["SQ"][0][1]="VBZ";
 			nonTerminalInfo["SQ"][0][2]="VBD";
 			nonTerminalInfo["SQ"][0][3]="VBP";
 			nonTerminalInfo["SQ"][0][4]="VB";
 			nonTerminalInfo["SQ"][0][5]="MD";
 			nonTerminalInfo["SQ"][0][6]="AUX";
 			nonTerminalInfo["SQ"][0][7]="AUXG";
 			nonTerminalInfo["SQ"][0][8]="VP";
 			nonTerminalInfo["SQ"][0][9]="SQ";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["SQ"]=new std::string*[1];
 			 			nonTerminalInfo["SQ"][0]=new std::string[10];
 			 			nonTerminalInfo["SQ"][0][0]="left";
 			 			nonTerminalInfo["SQ"][0][1]="VP";
 			 			nonTerminalInfo["SQ"][0][2]="SQ";
 			 			nonTerminalInfo["SQ"][0][3]="ADJP";
 			 			nonTerminalInfo["SQ"][0][4]="VB";
 			 			nonTerminalInfo["SQ"][0][5]="VBZ";
 			 			nonTerminalInfo["SQ"][0][6]="VBD";
 			 			nonTerminalInfo["SQ"][0][7]="VBP";
 			 			nonTerminalInfo["SQ"][0][8]="MD";
 			 			nonTerminalInfo["SQ"][0][9]="AUX";
 			 			nonTerminalInfo["SQ"][0][10]="AUX";
 			 			nonTerminalInfo["SQ"][0][11]="AUXG";
 			
			//nonTerminalInfo.put("UCP", new String[][]{{"right"}});
 			/*nonTerminalInfo["UCP"]=new std::string*[1];
 			nonTerminalInfo["UCP"][0]=new std::string[1];
 			nonTerminalInfo["UCP"][0][0]="right";*/
 			 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["UCP"]=new std::string*[1];
 			 			nonTerminalInfo["UCP"][0]=new std::string[1];
 			 			nonTerminalInfo["UCP"][0][0]="left";
 			
 			// below is weird!! Make 2 lists, one for good and one for bad heads?? ANOTHER COMMENT IN STF CODE
		    // VP: added AUX and AUXG to work with Charniak tags AND ANOTHER ONE
 			
 			//nonTerminalInfo.put("VP", new String[][]{{"left", "TO", "VBD", "VBN", "MD", "VBZ", "VB", "VBG", "VBP", "VP", "AUX", "AUXG", "ADJP", "JJP", "NN", "NNS", "JJ", "NP", "NNP"}});
 			nonTerminalInfo["VP"]=new std::string*[1];
 			nonTerminalInfo["VP"][0]=new std::string[19];
 			nonTerminalInfo["VP"][0][0]="left";
 			nonTerminalInfo["VP"][0][1]="TO";
 			nonTerminalInfo["VP"][0][2]="VBD";
 			nonTerminalInfo["VP"][0][3]="VBN";
 			nonTerminalInfo["VP"][0][4]="MD";
 			nonTerminalInfo["VP"][0][5]="VBZ";
 			nonTerminalInfo["VP"][0][6]="VB";
 			nonTerminalInfo["VP"][0][7]="VBG";
 			nonTerminalInfo["VP"][0][8]="VBP";
 			nonTerminalInfo["VP"][0][9]="VP";
 			nonTerminalInfo["VP"][0][10]="AUX";
 			nonTerminalInfo["VP"][0][11]="AUXG";
 			nonTerminalInfo["VP"][0][12]="ADJP";
 			nonTerminalInfo["VP"][0][13]="JJP";
 			nonTerminalInfo["VP"][0][14]="NN";
 			nonTerminalInfo["VP"][0][15]="NNS";
 			nonTerminalInfo["VP"][0][16]="JJ";
 			nonTerminalInfo["VP"][0][17]="NP";
 			nonTerminalInfo["VP"][0][18]="NNP";
 			
 			//nonTerminalInfo.put("WHADJP", new String[][]{{"left", "WRB", "WHADVP", "RB", "JJ", "ADJP", "JJP", "JJR"}});
 			
 			/*nonTerminalInfo["WHADJP"]=new std::string*[1];
 			nonTerminalInfo["WHADJP"][0]=new std::string[8];
 			nonTerminalInfo["WHADJP"][0][0]="left";
 			nonTerminalInfo["WHADJP"][0][1]="WRB";
 			nonTerminalInfo["WHADJP"][0][2]="WHADVP";
 			nonTerminalInfo["WHADJP"][0][3]="RB";
 			nonTerminalInfo["WHADJP"][0][4]="JJ";
 			nonTerminalInfo["WHADJP"][0][5]="ADJP";
 			nonTerminalInfo["WHADJP"][0][6]="JJP";
 			nonTerminalInfo["WHADJP"][0][7]="JJR";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["WHADJP"]=new std::string*[3];
 			 			nonTerminalInfo["WHADJP"][0]=new std::string[4];
 			 			nonTerminalInfo["WHADJP"][0][0]="left";
 			 			nonTerminalInfo["WHADJP"][0][1]="ADJP";
 			 			nonTerminalInfo["WHADJP"][0][2]="JJ";
 			 			nonTerminalInfo["WHADJP"][0][3]="JJR";
 			 			nonTerminalInfo["WHADJP"][1]=new std::string[2];
 			 			nonTerminalInfo["WHADJP"][1][0]="right";
 			 			nonTerminalInfo["WHADJP"][1][1]="RB";
 			 			nonTerminalInfo["WHADJP"][2]=new std::string[2];
 			 			nonTerminalInfo["WHADJP"][2][0]="right";
 			
 			
 			  //nonTerminalInfo.put("WHADVP", new String[][]{{"right", "WRB", "WHADVP"}}); 
 			/*nonTerminalInfo["WHADVP"]=new std::string*[1];
 			nonTerminalInfo["WHADVP"][0]=new std::string[3];
 			nonTerminalInfo["WHADVP"][0][0]="right";
 			nonTerminalInfo["WHADVP"][0][1]="WRB";
 			nonTerminalInfo["WHADVP"][0][2]="WHADVP";*/
 			 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			 nonTerminalInfo["WHADVP"]=new std::string*[1];
 			 nonTerminalInfo["WHADVP"][0]=new std::string[5];
 			 nonTerminalInfo["WHADVP"][0][0]="rightdis";
 			 nonTerminalInfo["WHADVP"][0][1]="WRB";
 			 nonTerminalInfo["WHADVP"][0][2]="WHADVP";
 			nonTerminalInfo["WHADVP"][0][3]="RB";
 			nonTerminalInfo["WHADVP"][0][4]="JJ";
 			 			//nonTerminalInfo.put("WHNP", new String[][]{{"left", "WDT", "WP", "WP$", "WHADJP", "WHPP", "WHNP"}});
 			/*nonTerminalInfo["WHNP"]=new std::string*[1];
 			nonTerminalInfo["WHNP"][0]=new std::string[7];
 			nonTerminalInfo["WHNP"][0][0]="left";
 			nonTerminalInfo["WHNP"][0][1]="WDT";
 			nonTerminalInfo["WHNP"][0][2]="WP";
 			nonTerminalInfo["WHNP"][0][3]="WP$";
 			nonTerminalInfo["WHNP"][0][4]="WHADJP";
 			nonTerminalInfo["WHNP"][0][5]="WHPP";
 			nonTerminalInfo["WHNP"][0][6]="WHNP";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["WHNP"]=new std::string*[4];
 			 			nonTerminalInfo["WHNP"][0]=new std::string[8];
 			 			nonTerminalInfo["WHNP"][0][0]="rightdis";
 			 			nonTerminalInfo["WHNP"][0][1]="NN";
 			 			nonTerminalInfo["WHNP"][0][2]="NNP";
 			 			nonTerminalInfo["WHNP"][0][3]="NNS";
 			 			nonTerminalInfo["WHNP"][0][4]="NX";
 			 			nonTerminalInfo["WHNP"][0][5]="NML";
 			 			nonTerminalInfo["WHNP"][0][6]="JJR";
 			 			nonTerminalInfo["WHNP"][0][7]="WP";
 			 			nonTerminalInfo["WHNP"][1]=new std::string[3];
 			 			nonTerminalInfo["WHNP"][1][0]="left";
 			 			nonTerminalInfo["WHNP"][1][1]="WHNP";
 			 			nonTerminalInfo["WHNP"][1][2]="NP";
 			 			nonTerminalInfo["WHNP"][2]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][2][0]="rightdis";
 			 			nonTerminalInfo["WHNP"][2][1]="$";
 			 			nonTerminalInfo["WHNP"][2][2]="ADJP";
 			 			nonTerminalInfo["WHNP"][2][3]="PRN";
 			 			nonTerminalInfo["WHNP"][2][4]="FW";
 			 			nonTerminalInfo["WHNP"][3]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][3][0]="right";
 			 			nonTerminalInfo["WHNP"][3][1]="CD";
 			 			nonTerminalInfo["WHNP"][4]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][4][0]="rightdis";
 			 			nonTerminalInfo["WHNP"][4][1]="JJ";
 			 			nonTerminalInfo["WHNP"][4][2]="JJS";
 			 			nonTerminalInfo["WHNP"][4][3]="RB";
 			 			nonTerminalInfo["WHNP"][4][4]="QP";
 			 			nonTerminalInfo["WHNP"][5]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][5][0]="left";
 			 			nonTerminalInfo["WHNP"][5][1]="WHPP";
 			 			nonTerminalInfo["WHNP"][5][2]="WHADJP";
 			 			nonTerminalInfo["WHNP"][5][3]="WP$";
 			 			nonTerminalInfo["WHNP"][5][4]="WDT";
 
 			 			
 			 			
 			//nonTerminalInfo.put("WHPP", new String[][]{{"right", "IN", "TO", "FW"}});
 			nonTerminalInfo["WHPP"]=new std::string*[1];
 			nonTerminalInfo["WHPP"][0]=new std::string[4];
 			nonTerminalInfo["WHPP"][0][0]="right";
 			nonTerminalInfo["WHPP"][0][1]="IN";
 			nonTerminalInfo["WHPP"][0][2]="TO";
 			nonTerminalInfo["WHPP"][0][3]="FW";
 			
 			//nonTerminalInfo.put("X", new String[][]{{"right", "S", "VP", "ADJP", "JJP", "NP", "SBAR", "PP", "X"}});
 			nonTerminalInfo["X"]=new std::string*[1];
 			nonTerminalInfo["X"][0]=new std::string[9];
 			nonTerminalInfo["X"][0][0]="right";
 			nonTerminalInfo["X"][0][1]="S";
 			nonTerminalInfo["X"][0][2]="VP";
 			nonTerminalInfo["X"][0][3]="ADJP";
 			nonTerminalInfo["X"][0][4]="JJP";
 			nonTerminalInfo["X"][0][5]="NP";
 			nonTerminalInfo["X"][0][6]="SBAR";
 			nonTerminalInfo["X"][0][7]="PP";
 			nonTerminalInfo["X"][0][8]="X";
 			
 			
 			//nonTerminalInfo.put("NP", new String[][]{{"rightdis", "NN", "NNP", "NNPS", "NNS", "NML", "NX", "POS", "JJR"}, {"left", "NP", "PRP"}, {"rightdis", "$", "ADJP", "JJP", "PRN", "FW"}, {"right", "CD"}, {"rightdis", "JJ", "JJS", "RB", "QP", "DT", "WDT", "RBR", "ADVP"}});
 			/*nonTerminalInfo["NP"]=new std::string*[5];
 			nonTerminalInfo["NP"][0]=new std::string[10];
 			nonTerminalInfo["NP"][0][0]="rightdis";
 			nonTerminalInfo["NP"][0][1]="NN";
 			nonTerminalInfo["NP"][0][2]="NNP";
 			nonTerminalInfo["NP"][0][3]="NNPS";
 			nonTerminalInfo["NP"][0][4]="NNS";
 			nonTerminalInfo["NP"][0][5]="NML";
 			nonTerminalInfo["NP"][0][6]="NX";
 			nonTerminalInfo["NP"][0][7]="NML";
 			nonTerminalInfo["NP"][0][8]="POS";
 			nonTerminalInfo["NP"][0][9]="JJR";
 			nonTerminalInfo["NP"][1]=new std::string[3];
 			nonTerminalInfo["NP"][1][0]="left";
 			nonTerminalInfo["NP"][1][1]="NP";
 			nonTerminalInfo["NP"][1][2]="PRP";
 			nonTerminalInfo["NP"][2]=new std::string[6];
 			nonTerminalInfo["NP"][2][0]="rightdis";
 			nonTerminalInfo["NP"][2][1]="$";
 			nonTerminalInfo["NP"][2][2]="ADJP";
 			nonTerminalInfo["NP"][2][3]="JJP";
 			nonTerminalInfo["NP"][2][4]="PRN";
 			nonTerminalInfo["NP"][2][5]="FW";
 			nonTerminalInfo["NP"][3]=new std::string[2];
 			nonTerminalInfo["NP"][3][0]="right";
 			nonTerminalInfo["NP"][3][1]="CD";
 			nonTerminalInfo["NP"][4]=new std::string[9];
 			nonTerminalInfo["NP"][4][0]="rightdis";
 			nonTerminalInfo["NP"][4][1]="JJ";			
 			nonTerminalInfo["NP"][4][2]="JJS";
 			nonTerminalInfo["NP"][4][3]="RB";
 			nonTerminalInfo["NP"][4][4]="QP";
 			nonTerminalInfo["NP"][4][5]="DT";
 			nonTerminalInfo["NP"][4][6]="WDT";
 			nonTerminalInfo["NP"][4][7]="RBR";
 			nonTerminalInfo["NP"][4][8]="ADVP";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 					nonTerminalInfo["NP"]=new std::string*[6];
 			 		nonTerminalInfo["NP"][0]=new std::string[8];
 			 		nonTerminalInfo["NP"][0][0]="rightdis";
 			 		nonTerminalInfo["NP"][0][1]="NN";
 			 		nonTerminalInfo["NP"][0][2]="NNP";
 			 		nonTerminalInfo["NP"][0][3]="NNPS";
 			 		nonTerminalInfo["NP"][0][4]="NNS";
 			 		nonTerminalInfo["NP"][0][5]="NML";
 			 		nonTerminalInfo["NP"][0][6]="NX";
 			 		nonTerminalInfo["NP"][0][7]="NML";
 			 		//nonTerminalInfo["NP"][0][8]="POS";
 			 		nonTerminalInfo["NP"][0][8]="JJR";
 			 		nonTerminalInfo["NP"][1]=new std::string[3];
 			 		nonTerminalInfo["NP"][1][0]="left";
 			 		nonTerminalInfo["NP"][1][1]="NP";
 			 		nonTerminalInfo["NP"][1][2]="PRP";
 			 		nonTerminalInfo["NP"][2]=new std::string[4];
 			 		nonTerminalInfo["NP"][2][0]="rightdis";
 			 		nonTerminalInfo["NP"][2][1]="$";
 			 		nonTerminalInfo["NP"][2][2]="ADJP";
 			 		/*nonTerminalInfo["NP"][2][3]="JJP";
 			 		nonTerminalInfo["NP"][2][4]="PRN";*/
 			 		nonTerminalInfo["NP"][2][3]="FW";
 			 		nonTerminalInfo["NP"][3]=new std::string[2];
 			 		nonTerminalInfo["NP"][3][0]="right";
 			 		nonTerminalInfo["NP"][3][1]="CD";
 			 		nonTerminalInfo["NP"][4]=new std::string[11];
 			 		nonTerminalInfo["NP"][4][0]="rightdis";
 			 		nonTerminalInfo["NP"][4][1]="JJ";			
 			 		nonTerminalInfo["NP"][4][2]="JJS";
 			 		//nonTerminalInfo["NP"][4][3]="RB";
 			 		//nonTerminalInfo["NP"][4][4]="QP";
 			 		//nonTerminalInfo["NP"][4][5]="DT";
 			 		//nonTerminalInfo["NP"][4][6]="WDT";
 			 		//nonTerminalInfo["NP"][4][7]="RBR";
 			 		//nonTerminalInfo["NP"][4][8]="ADVP";
 			 		nonTerminalInfo["NP"][4][3]="QP";
 			 		nonTerminalInfo["NP"][4][4]="DT";
 			 		nonTerminalInfo["NP"][4][5]="WDT";
 			 		nonTerminalInfo["NP"][4][6]="NML";
 			 		nonTerminalInfo["NP"][4][7]="PRN";
 			 		nonTerminalInfo["NP"][4][8]="RB";
 			 		nonTerminalInfo["NP"][4][9]="RBR";
 			 		nonTerminalInfo["NP"][4][10]="ADVP";
 			 		nonTerminalInfo["NP"][5]=new std::string[2];
 			 		nonTerminalInfo["NP"][5][0]="left";
 			 		nonTerminalInfo["NP"][5][1]="POS";
 			
 			
 			// TODO: seems JJ should be head of NML in this case:
		    // (NP (NML (JJ former) (NML Red Sox) (JJ great)) (NNP Luis) (NNP Tiant)),
 		    //nonTerminalInfo.put("NML", new String[][]{{"rightdis", "NN", "NNP", "NNPS", "NNS", "NX", "NML", "POS", "JJR"}, {"left", "NP", "PRP"}, {"rightdis", "$", "ADJP", "JJP", "PRN"}, {"right", "CD"}, {"rightdis", "JJ", "JJS", "RB", "QP", "DT", "WDT", "RBR", "ADVP"}}); 			
 			nonTerminalInfo["NML"]=new std::string*[5];
 			nonTerminalInfo["NML"][0]=new std::string[9];
 			nonTerminalInfo["NML"][0][0]="rightdis";
 			nonTerminalInfo["NML"][0][1]="NN";
 			nonTerminalInfo["NML"][0][2]="NNP";
 			nonTerminalInfo["NML"][0][3]="NNPS";
 			nonTerminalInfo["NML"][0][4]="NNS";
 			nonTerminalInfo["NML"][0][5]="NX";
 			nonTerminalInfo["NML"][0][6]="NML";
 			nonTerminalInfo["NML"][0][7]="POS";
 			nonTerminalInfo["NML"][0][8]="JJR";
 			nonTerminalInfo["NML"][1]=new std::string[7];
 			nonTerminalInfo["NML"][1][0]="left";
 			nonTerminalInfo["NML"][1][1]="NP";
 			nonTerminalInfo["NML"][1][2]="PRP";
 			nonTerminalInfo["NML"][2]=new std::string[5];
 			nonTerminalInfo["NML"][2][0]="rightdis";
 			nonTerminalInfo["NML"][2][1]="$";
 			nonTerminalInfo["NML"][2][2]="ADJP";
 			nonTerminalInfo["NML"][2][3]="JJP";
 			nonTerminalInfo["NML"][2][4]="PRN";
 			nonTerminalInfo["NML"][3]=new std::string[2];
 			nonTerminalInfo["NML"][3][0]="right";
 			nonTerminalInfo["NML"][3][1]="CD";
 			nonTerminalInfo["NML"][4]=new std::string[9];
 			nonTerminalInfo["NML"][4][0]="rightdis";
 			nonTerminalInfo["NML"][4][1]="JJ";			
 			nonTerminalInfo["NML"][4][2]="JJS";
 			nonTerminalInfo["NML"][4][3]="RB";
 			nonTerminalInfo["NML"][4][4]="QP";
 			nonTerminalInfo["NML"][4][5]="DT";
 			nonTerminalInfo["NML"][4][6]="WDT";
 			nonTerminalInfo["NML"][4][7]="RBR";
 			nonTerminalInfo["NML"][4][8]="ADVP";
 			
			//nonTerminalInfo.put("POSSP", new String[][]{{"right", "POS"}});
 			nonTerminalInfo["POSSP"]=new std::string*[1];
 			nonTerminalInfo["POSSP"][0]=new std::string[2];
 			nonTerminalInfo["POSSP"][0][0]="right";
 			nonTerminalInfo["POSSP"][0][1]="POS";
 			
 			/* HJT: Adding the following to deal with oddly formed data in (for example) the Brown corpus COMMENT IN STF CODE*/
 			/* nonTerminalInfo.put("ROOT", new String[][]{{"left", "S", "SQ", "SINV", "SBAR", "FRAG"}});*/
 			
 			nonTerminalInfo["ROOT"]=new std::string*[1];
 			nonTerminalInfo["ROOT"][0]=new std::string[2];
 			nonTerminalInfo["ROOT"][0][0]="left";
 			nonTerminalInfo["ROOT"][0][1]="S";
 			nonTerminalInfo["ROOT"][0][2]="SQ";
 			nonTerminalInfo["ROOT"][0][3]="SINV";
 			nonTerminalInfo["ROOT"][0][4]="SBAR";
 			nonTerminalInfo["ROOT"][0][5]="FRAG";
 			

			//nonTerminalInfo.put("TYPO", new String[][]{{"left", "NN", "NP", "NML", "NNP", "NNPS", "TO",
			 //     "VBD", "VBN", "MD", "VBZ", "VB", "VBG", "VBP", "VP", "ADJP", "JJP", "FRAG"}}); // for Brown (Roger) COMMENT in STF CODE

 			nonTerminalInfo["TYPO"]=new std::string*[1];
 			nonTerminalInfo["TYPO"][0]=new std::string[2];
 			nonTerminalInfo["TYPO"][0][0]="left";
 			nonTerminalInfo["TYPO"][0][1]="NN";
 			nonTerminalInfo["TYPO"][0][2]="NP";
 			nonTerminalInfo["TYPO"][0][3]="NML";
 			nonTerminalInfo["TYPO"][0][4]="NNP";
 			nonTerminalInfo["TYPO"][0][5]="NNPS";
 			nonTerminalInfo["TYPO"][0][6]="TO";
 			nonTerminalInfo["TYPO"][0][7]="VBD";
 			nonTerminalInfo["TYPO"][0][8]="VBN";
 			nonTerminalInfo["TYPO"][0][9]="MD";
 			nonTerminalInfo["TYPO"][0][11]="VBZ";
 			nonTerminalInfo["TYPO"][0][12]="VB";
 			nonTerminalInfo["TYPO"][0][13]="VBG";
 			nonTerminalInfo["TYPO"][0][14]="VBP";
 			nonTerminalInfo["TYPO"][0][15]="VP";
 			nonTerminalInfo["TYPO"][0][16]="ADJP";
 			nonTerminalInfo["TYPO"][0][17]="JJP";
 			nonTerminalInfo["TYPO"][0][18]="FRAG";
 			
 			//nonTerminalInfo.put("ADV", new String[][]{{"right", "RB", "RBR", "RBS", "FW",
		      //"ADVP", "TO", "CD", "JJR", "JJ", "IN", "NP", "NML", "JJS", "NN"}});
 			nonTerminalInfo["ADV"]=new std::string*[1];
 			nonTerminalInfo["ADV"][0]=new std::string[15];
 			nonTerminalInfo["ADV"][0][0]="right";
 			nonTerminalInfo["ADV"][0][1]="RB";
 			nonTerminalInfo["ADV"][0][2]="RBR";
 			nonTerminalInfo["ADV"][0][3]="RBS";
 			nonTerminalInfo["ADV"][0][4]="FW";
 			nonTerminalInfo["ADV"][0][5]="ADVP";
 			nonTerminalInfo["ADV"][0][6]="TO";
 			nonTerminalInfo["ADV"][0][7]="CD";
 			nonTerminalInfo["ADV"][0][8]="JJR";
 			nonTerminalInfo["ADV"][0][9]="JJ";
 			nonTerminalInfo["ADV"][0][10]="IN";
 			nonTerminalInfo["ADV"][0][11]="NP";
 			nonTerminalInfo["ADV"][0][12]="NML";
 			nonTerminalInfo["ADV"][0][13]="JJS";
 			nonTerminalInfo["ADV"][0][14]="NN";
 			
 		   // SWBD
 		   //nonTerminalInfo.put("EDITED", new String[][] {{"left"}});  // crap rule for Switchboard (if don't delete EDITED nodes) Comment in stf code
 			nonTerminalInfo["EDITED"]=new std::string*[1];
 			nonTerminalInfo["EDITED"][0]=new std::string[1];
 			nonTerminalInfo["EDITED"][0][0]="left";
 			
 			
 			// in sw2756, a "VB". (copy "VP" to handle this problem, though should really fix it on reading)
		    //nonTerminalInfo.put("VB", new String[][]{{"left", "TO", "VBD", "VBN", "MD", "VBZ", "VB", "VBG", "VBP", "VP", "AUX", "AUXG", "ADJP", "JJP", "NN", "NNS", "JJ", "NP", "NNP"}});
 			nonTerminalInfo["VB"]=new std::string*[1];
 			nonTerminalInfo["VB"][0]=new std::string[19];
 			nonTerminalInfo["VB"][0][0]="left";
 			nonTerminalInfo["VB"][0][1]="TO";
 			nonTerminalInfo["VB"][0][2]="VBD";
 			nonTerminalInfo["VB"][0][3]="VBN";
 			nonTerminalInfo["VB"][0][4]="MD";
 			nonTerminalInfo["VB"][0][5]="VBZ";
 			nonTerminalInfo["VB"][0][6]="VB";
 			nonTerminalInfo["VB"][0][7]="VBG";
 			nonTerminalInfo["VB"][0][8]="VBP";
 			nonTerminalInfo["VB"][0][9]="VP";
 			nonTerminalInfo["VB"][0][10]="AUX";
 			nonTerminalInfo["VB"][0][11]="AUXG";
 			nonTerminalInfo["VB"][0][12]="ADJP";
 			nonTerminalInfo["VB"][0][13]="JJP";
 			nonTerminalInfo["VB"][0][14]="NN";
 			nonTerminalInfo["VB"][0][15]="NNS";
 			nonTerminalInfo["VB"][0][16]="JJ";
 			nonTerminalInfo["VB"][0][17]="NP";
 			nonTerminalInfo["VB"][0][18]="NNP";
 			
 			//nonTerminalInfo.put("META", new String[][] {{"left"}});  // rule for OntoNotes, but maybe should just be deleted in TreeReader?? //Comment in STF CODE
 			
 			nonTerminalInfo["META"]=new std::string*[1];
 			nonTerminalInfo["META"][0]=new std::string[1];
 			nonTerminalInfo["META"][0][0]="left";
 			
 			//nonTerminalInfo.put("XS", new String[][] {{"right", "IN"}});*/ // rule for new structure in QP, introduced by Stanford in QPTreeTransformer
 			/*nonTerminalInfo["XS"]=new std::string*[1];
 			nonTerminalInfo["XS"][0]=new std::string[2];
 			nonTerminalInfo["XS"][0][0]="right";
 			nonTerminalInfo["XS"][0][1]="IN";*/
 			//It didn't change in the ruleChanges() method, but still it was there
 			nonTerminalInfo["XS"]=new std::string*[1];
 			nonTerminalInfo["XS"][0]=new std::string[2];
 			nonTerminalInfo["XS"][0][0]="right";
 			nonTerminalInfo["XS"][0][1]="IN";
 			
 			nonTerminalInfo["EMBED"]=new std::string*[1];
 			nonTerminalInfo["EMBED"][0]=new std::string[2];
 			nonTerminalInfo["EMBED"][0][0]="right";
 			nonTerminalInfo["EMBED"][0][1]="INTJ";
 			
 			//Comments in stf code
 			// nonTerminalInfo.put(null, new String[][] {{"left"}});  // rule for OntoNotes from Michel, but it would be better to fix this in TreeReader or to use a default rule? 
 			// todo: Uncomment this line if we always want to take the leftmost if no head rule is defined for the mother category.
 			// defaultRule = defaultLeftRule; // Don't exception, take leftmost if no rule defined for a certain parent category
 			
 			
 			
 			
 			
 		}
 		

 		
 	public:
 		//Methods from semantic head finder
 		
 		/*
 		Tree determineNonTrivialHead(Tree t, Tree parent) {
 			...
 		}
 		*/
 		
 		/*int postOperationFix(int headIdz, Tree[] daughterTrees) {
 			
 		}*/
 		
 		/*boolean isExistential(Tree t, Tree parent) {
 			
 		}*/
 		/*bool isWhQ() {
 			//CStateNode* c;
 			return false;
 		}*/
 		/*boolean hasPassiveProgressiveAuxiliary(Tree[] kids, Set<String> verbalSet--> String* verbalSet) {
 		 			
 		}*/
 		
 		/*boolean vpContainsParticiple(Tree t) {
 		  
 		 }
 		*/
 		
 		/*boolean isVerbalAuxiliary(Tree t) {
 			
 		}*/
    
 		/*boolean hasVerbalAuxiliary(Tree[] kids, Set<String> verbalSet) {
 			
 		}*/

 };
 







#endif
