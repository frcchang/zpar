1. Build.
	Go into the src dir then make.
	  a. cd src
	  b. make -f Makefile
	After make, an executable named Tagger would be created in src/


2. Training.
	You can train your own model with the following command:
	   src/Tagger   training   configureFile
	for example:
	   src/Tagger  training  config/conf_sample.txt	 
	where configFile specify some of the parameters used during training.
	There are some examples of the configureFile in config/
	(When training your own model, you just need to specify the path of the training set, 
	 development set and the path of the trained model.
	 For others parameters, use their default value is OK)

	lines start with :: will be ignored
	Take conf/conf.txt as an example:
		trainPath    : specify the training data
		devPath      : specify the development set, you can specify several development set
		testPath     : similar as devPath.
		strLogDir    : a log file will be generated during the training process,
		                 the log file records tagging accuracy of each epoch on development and test sets.
		strModelPath : specify the directory where the model will be generated
		nRound       : number of maximum training epoch.
		prefix       : the prefix of the name of the trained model
		strRBMPrefix : specify the "common prefix" of the word-representation RBM, 
									   which is used as features to improve tagging accuracy.
									 
									   Example of "common Prefix":
									   WRRBM contains several modules, for example:
										  wrrbm_abc.dict  wrrbm_abc.model  wrrbm_abc.random
									   the common prefix is "wrrbm_abc".
									 using WRRBM will cause both training and testing slow,
									 but the model will improve web-domain tagging accuracy.

									 if you want to train a tagger on standard WSJ data set,
									 you can choose to ignore wrrbm by add "::" at the begining
									 of the line of this parameter.

		bEnTagger    : "true" denotes English tagger, 
		               "false" means Chinese tagger(currently not supported)
		bEarlyUpdate : "true" denotes whenever a word is incorrectly tagged,
		                parameters will be updated and the rest of the sentence is ignored.
		
		fMargin      : since we use a margin loss (Ma et al., 2014) to train the model, 
		               this parameter specify the value of the margin
		fRate        : learning rate, currently, we do not use weight decay.
		vIHSize      : size of the feature embedding, corresponding to the deminsion of the hidden layer size of
		               the sparse feature module described in section 3.2 of (Ma et al., 2014)
    vIHType      : "linear", linear projection layer


3. Tagging
		src/Tagger  tagging  modelPrefix  inputFile  outputFile
		
		After training, several files will be generated at the location specified by
		"strModelPath", these files share a common prefix, for example:
		
		WebTagger_AvgParam.confg   WebTagger_AvgParam.model  ....
		the common prefix is "WebTagger_AvgParam"

		"modelPrefix" is that common prefix.


Data format:
  See the sample data provided at sampleData/.

	Each line of the data file is a token which contains three columns, 
	   Word  generalizedForm  POS
	Word: word form
	generalizedForm: lower cased Word, digit are converted to #DIG...
	                 scripts for such processing will coming soon.
	POS: part of speech, when tagging new data, this column would be ignored by the tagger  
	A sentence contains several tokens.	Sentences are separated by empty line.
	

How to cite:
  Either cite the URL or the paper 
	"Tagging the web: building robust web tagger using neural network"
	is OK.

	
	
