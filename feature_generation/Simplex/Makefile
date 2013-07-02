.PHONY : clean $(COMMONDIR)/lib/util.a check-dir

export DEBUG=0
export OBJDIR=$(shell pwd)/obj
export TARGETDIR=$(shell pwd)/bin
export COMMONDIR=$(shell pwd)/common
export OUTPUTDIR=$(shell pwd)/output
export DATA=$(shell pwd)/data
export SRC=$(shell pwd)/src
export FEATUREDIR=$(shell pwd)/feature
export LDA=$(shell pwd)/lda
export LDADIR=$(shell pwd)/package/GibbsLDA++-0.2/

export FIX_SUFFIX=.fix
export CLEAN_SUFFIX=.final


LDANTOPIC=100
LDAITER=2000

PAPERCSV=$(OUTPUTDIR)/Paper.csv$(CLEAN_SUFFIX)
PAPERAUTHORCSV=$(OUTPUTDIR)/PaperAuthor.csv$(CLEAN_SUFFIX)
AUTHORCSV=$(OUTPUTDIR)/Author.csv$(CLEAN_SUFFIX)
TRAIN=$(DATA)/Train.csv
VALID=$(DATA)/Valid.csv
CLEANTEST=$(OUTPUTDIR)/CleanTest.csv
INTERNAL_TRAIN=$(DATA)/internal_train
INTERNAL_VALID=$(DATA)/internal_valid

MERGER=./py/merge_feature_files.py
SPARSE=./py/dense_to_sparse.py
SPLIT=./py/split_internal.py

CC=g++
CFLAG=-c -Wall -Wextra -Wshadow -Wconversion -Werror -I$(COMMONDIR) -I$(SRC)
LDFLAG=-lm

ifeq ($(DEBUG), 1)
	CFLAG +=-g -DDEBUG
else
	CFLAG +=-O2
endif

all : basic-feature

check-dir : 
	mkdir -p $(OBJDIR)
	mkdir -p $(FEATUREDIR)
	mkdir -p $(TARGETDIR)
	mkdir -p $(OUTPUTDIR)
	mkdir -p $(LDA)

clean : 
	rm -f $(OBJDIR)/* $(FEATUREDIR)/* $(TARGETDIR)/* $(OUTPUTDIR)/* $(LDA)/*


utility : $(TARGETDIR)/make-submission

#Fix Paper.csv PaperAuthor.csv

$(OUTPUTDIR)/Paper.csv$(FIX_SUFFIX): $(DATA)/Paper.csv ./py/FixPaper.py
	./py/FixPaper.py $(DATA)/Paper.csv $@

$(OUTPUTDIR)/PaperAuthor.csv$(FIX_SUFFIX) : $(DATA)/PaperAuthor.csv ./py/FixPaperAuthor.py
	./py/FixPaperAuthor.py $(DATA)/PaperAuthor.csv $@

$(OUTPUTDIR)/Author.csv$(FIX_SUFFIX) : $(DATA)/Author.csv
	cp $< $@

#clean Paper.csv PaperAuthor.csv Author.csv

$(OUTPUTDIR)/%.csv$(CLEAN_SUFFIX) : $(OUTPUTDIR)/%.csv$(FIX_SUFFIX) ./py/clean_data.py
	./py/clean_data.py $< $@

data : $(OUTPUTDIR)/Paper.csv$(CLEAN_SUFFIX) $(OUTPUTDIR)/Author.csv$(CLEAN_SUFFIX) $(OUTPUTDIR)/PaperAuthor.csv$(CLEAN_SUFFIX)

$(TARGETDIR)/% : $(OBJDIR)/%.o $(COMMONDIR)/lib/util.a
	$(CC) $(LDFLAG) $^ -o $@

$(OBJDIR)/%.o : $(SRC)/%.cpp $(SRC)/kdd2013.h
	$(CC) $(CFLAG) $< -o $@

$(OBJDIR)/coauthor_network_steps.o : $(SRC)/coauthor_network_steps.cpp
	$(CC) $(CFLAG) -std=c++0x $< -o $@

$(OUTPUTDIR)/author-list : $(TARGETDIR)/generate-author-list $(PAPERAUTHORCSV)
	$< $(PAPERAUTHORCSV) $@
	
$(OUTPUTDIR)/onestep.list : $(TARGETDIR)/coauthor_network_steps $(OUTPUTDIR)/author-list
	$< $(OUTPUTDIR)/author-list 1 > $@
	
$(OUTPUTDIR)/Train_JMY_feature : $(TARGETDIR)/expand-author-paper-feature-ex $(OUTPUTDIR)/onestep.list $(AUTHORCSV) $(PAPERAUTHORCSV) $(PAPERCSV) $(TRAIN) $(VALID) $(OUTPUTDIR)/onestep.list
	$< $(AUTHORCSV) $(PAPERAUTHORCSV) $(PAPERCSV) $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_JMY_feature $(OUTPUTDIR)/Valid_JMY_feature $(OUTPUTDIR)/Valid.meta /dev/null /dev/null $(OUTPUTDIR)/onestep.list

$(OUTPUTDIR)/Valid_JMY_feature : $(OUTPUTDIR)/Train_JMY_feature

$(OUTPUTDIR)/Valid_SYY_feature : $(OUTPUTDIR)/Train_SYY_feature

$(OUTPUTDIR)/Train_SYY_feature : ./py/features_gen.py data
	$< $(OUTPUTDIR)/Train_SYY_feature $(OUTPUTDIR)/Valid_SYY_feature $(DATA) $(TRAIN) $(VALID)

$(COMMONDIR)/lib/util.a : 
	$(MAKE) -C $(COMMONDIR) 

$(FEATUREDIR)/Simplex.Basic.Train.dense : $(OUTPUTDIR)/Train_SYY_feature $(OUTPUTDIR)/Train_JMY_feature
	$(MERGER) $^ > $@

$(FEATUREDIR)/Simplex.Basic.Valid.dense : $(OUTPUTDIR)/Valid_SYY_feature $(OUTPUTDIR)/Valid_JMY_feature
	$(MERGER) $^ > $@

$(FEATUREDIR)/%.sparse : $(FEATUREDIR)/%.dense
	$(SPARSE) $< $@

basic-feature : $(FEATUREDIR)/Simplex.Basic.Train.dense $(FEATUREDIR)/Simplex.Basic.Valid.dense $(FEATUREDIR)/Simplex.Basic.Train.sparse $(FEATUREDIR)/Simplex.Basic.Valid.sparse

copy-basic-feature : basic-feature
	cp $(FEATUREDIR)/Simplex.Basic.Train.sparse $(TRAIN_FEATURE)
	cp $(FEATUREDIR)/Simplex.Basic.Valid.sparse $(VALID_FEATURE)


$(CLEANTEST) : ./py/generate_all_pair_from_paperauthor.py $(PAPERAUTHORCSV) $(PAPERCSV) $(AUTHORCSV)
	$< $(AUTHORCSV) $(PAPERCSV) $(PAPERAUTHORCSV) $(CLEANTEST)


$(OUTPUTDIR)/Clean_Train_JMY_feature : $(TARGETDIR)/expand-author-paper-feature-ex $(OUTPUTDIR)/onestep.list $(AUTHORCSV) $(PAPERAUTHORCSV) $(PAPERCSV) $(TRAIN) $(CLEANTEST) $(OUTPUTDIR)/onestep.list
	$< $(AUTHORCSV) $(PAPERAUTHORCSV) $(PAPERCSV) $(TRAIN) $(CLEANTEST) $(OUTPUTDIR)/Clean_Train_JMY_feature $(OUTPUTDIR)/Clean_Test_JMY_feature /dev/null /dev/null /dev/null $(OUTPUTDIR)/onestep.list

$(OUTPUTDIR)/Clean_Test_JMY_feature : $(OUTPUTDIR)/Clean_Train_JMY_feature

$(OUTPUTDIR)/Clean_Test_SYY_feature : $(OUTPUTDIR)/Clean_Train_SYY_feature


$(OUTPUTDIR)/Clean_Train_SYY_feature : ./py/clean_features_gen.py 
	$< $(OUTPUTDIR)/Clean_Train_SYY_feature $(OUTPUTDIR)/Clean_Test_SYY_feature $(DATA) $(TRAIN) $(CLEANTEST)

$(OUTPUTDIR)/Clean_Test.dense : $(OUTPUTDIR)/Clean_Test_SYY_feature $(OUTPUTDIR)/Clean_Test_JMY_feature
	$(MERGER) $^ > $@

$(OUTPUTDIR)/Clean_Train.dense : $(OUTPUTDIR)/Clean_Train_SYY_feature $(OUTPUTDIR)/Clean_Train_JMY_feature	
	$(MERGER) $^ > $@

$(OUTPUTDIR)/Clean_Internal.dense : $(OUTPUTDIR)/Clean_Train.dense
	$(SPLIT) $< $(OUTPUTDIR)/Clean_Internal.dense /dev/null

$(OUTPUTDIR)/internal.Clean.labels : ./py/sklearn-rf-classify.py $(OUTPUTDIR)/Clean_Internal.dense $(OUTPUTDIR)/Clean_Test.dense
	$< $(OUTPUTDIR)/Clean_Internal.dense $(OUTPUTDIR)/Clean_Test.dense 1000 sqrt $@

$(OUTPUTDIR)/Train.Clean.labels : ./py/sklearn-rf-classify.py $(OUTPUTDIR)/Clean_Train.dense $(OUTPUTDIR)/Clean_Test.dense
	$< $(OUTPUTDIR)/Clean_Train.dense $(OUTPUTDIR)/Clean_Test.dense 1000 sqrt $@

$(OUTPUTDIR)/PaperAuthor.csv.internal.Clean : ./py/make_clean_paper_author_csv.py $(INTERNAL_TRAIN) $(PAPERAUTHORCSV) $(CLEANTEST) $(OUTPUTDIR)/internal.Clean.labels 
	$< $(INTERNAL_TRAIN) $(PAPERAUTHORCSV) $(CLEANTEST) $(OUTPUTDIR)/internal.Clean.labels $@

$(OUTPUTDIR)/PaperAuthor.csv.Train.Clean : ./py/make_clean_paper_author_csv.py $(TRAIN) $(PAPERAUTHORCSV) $(CLEANTEST) $(OUTPUTDIR)/Train.Clean.labels 
	$< $(TRAIN) $(PAPERAUTHORCSV) $(CLEANTEST) $(OUTPUTDIR)/Train.Clean.labels $@

clean-paperauthor: $(OUTPUTDIR)/PaperAuthor.csv.Train.Clean $(OUTPUTDIR)/PaperAuthor.csv.internal.Clean

$(TARGETDIR)/tokenize : $(OBJDIR)/tokenize.o $(OBJDIR)/stemming.o 
	$(CC) $(LDFLAG) $^ -o $@

$(OUTPUTDIR)/paper_tokenized : $(TARGETDIR)/tokenize $(PAPERCSV)
	$< $(PAPERCSV) $@

$(OUTPUTDIR)/paper_token_stat : $(TARGETDIR)/get-token-statistic $(OUTPUTDIR)/paper_tokenized
	$< $(OUTPUTDIR)/paper_tokenized  $@ /dev/null

$(OUTPUTDIR)/PaperTitle : $(TARGETDIR)/get-doc-word $(OUTPUTDIR)/paper_tokenized  $(OUTPUTDIR)/paper_token_stat
	$< $(OUTPUTDIR)/paper_tokenized $(OUTPUTDIR)/paper_token_stat $(DATA)/StopWord $(OUTPUTDIR)/PaperTitle /dev/null $(OUTPUTDIR)/PaperTitleMeta

$(LDA)/PaperTitleForGibbs : $(OUTPUTDIR)/PaperTitle
	wc -l $(OUTPUTDIR)/PaperTitle | grep -o '[0-9]*' | head -n 1 > $(OUTPUTDIR)/tmp
	cat $(OUTPUTDIR)/tmp $(OUTPUTDIR)/PaperTitle > $(LDA)/PaperTitleForGibbs

$(LDA)/model-final.tassign : $(LDA)/PaperTitleForGibbs
	$(MAKE) -C $(LDADIR)
	${LDADIR}/src/lda -est -ntopics $(LDANTOPIC) -niters $(LDAITER) -savestep 500 -dfile $(LDA)/PaperTitleForGibbs


paper-title : $(LDA)/model-final.tassign 



$(OUTPUTDIR)/PaperTopic : $(TARGETDIR)/get-paper-topic $(LDA)/model-final.tassign $(OUTPUTDIR)/PaperTitleMeta 
	$< $(LDA)/model-final.tassign $(OUTPUTDIR)/PaperTitleMeta $(LDANTOPIC) $@

$(OUTPUTDIR)/AuthorTopicFeatureInternalClean : $(TARGETDIR)/get-topic-feature $(PAPERCSV) $(OUTPUTDIR)/PaperAuthor.csv.internal.Clean $(OUTPUTDIR)/PaperTopic 
	$< $(PAPERCSV) $(OUTPUTDIR)/PaperAuthor.csv.internal.Clean $(OUTPUTDIR)/PaperTopic $(OUTPUTDIR)/AuthorTopicFeatureInternalClean $(OUTPUTDIR)/PaperTopicFeatureInternalClean $(OUTPUTDIR)/ConferenceTopicFeatureInternalClean $(OUTPUTDIR)/JournalTopicFeatureInternalClean

$(OUTPUTDIR)/PaperTopicFeatureInternalClean : $(OUTPUTDIR)/AuthorTopicFeatureInternalClean

$(OUTPUTDIR)/ConferenceTopicFeatureInternalClean : $(OUTPUTDIR)/AuthorTopicFeatureInternalClean

$(OUTPUTDIR)/JournalTopicFeatureInternalClean : $(OUTPUTDIR)/AuthorTopicFeatureInternalClean

$(OUTPUTDIR)/AuthorTopicFeatureTrainClean : $(TARGETDIR)/get-topic-feature $(PAPERCSV) $(OUTPUTDIR)/PaperAuthor.csv.Train.Clean $(OUTPUTDIR)/PaperTopic
	$< $(PAPERCSV) $(OUTPUTDIR)/PaperAuthor.csv.Train.Clean $(OUTPUTDIR)/PaperTopic $(OUTPUTDIR)/AuthorTopicFeatureTrainClean $(OUTPUTDIR)/PaperTopicFeatureTrainClean $(OUTPUTDIR)/ConferenceTopicFeatureTrainClean $(OUTPUTDIR)/JournalTopicFeatureTrainClean

$(OUTPUTDIR)/PaperTopicFeatureTrainClean : $(OUTPUTDIR)/AuthorTopicFeatureTrainClean

$(OUTPUTDIR)/ConferenceTopicFeatureTrainClean : $(OUTPUTDIR)/AuthorTopicFeatureTrainClean

$(OUTPUTDIR)/JournalTopicFeatureTrainClean : $(OUTPUTDIR)/AuthorTopicFeatureTrainClean

$(OUTPUTDIR)/Train_topic_feature_internal_clean : $(TARGETDIR)/generate-topic-feature-file $(OUTPUTDIR)/AuthorTopicFeatureInternalClean $(OUTPUTDIR)/PaperTopicFeatureInternalClean $(OUTPUTDIR)/ConferenceTopicFeatureInternalClean $(OUTPUTDIR)/JournalTopicFeatureInternalClean $(TRAIN) $(VALID)
	$< $(LDANTOPIC) $(OUTPUTDIR)/AuthorTopicFeatureInternalClean $(OUTPUTDIR)/PaperTopicFeatureInternalClean $(OUTPUTDIR)/ConferenceTopicFeatureInternalClean $(OUTPUTDIR)/JournalTopicFeatureInternalClean $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_topic_feature_internal_clean $(OUTPUTDIR)/Valid_topic_feature_internal_clean

$(OUTPUTDIR)/Valid_topic_feature_internal_clean : $(OUTPUTDIR)/Train_topic_feature_internal_clean


$(OUTPUTDIR)/Train_topic_feature_Train_clean : $(TARGETDIR)/generate-topic-feature-file $(OUTPUTDIR)/AuthorTopicFeatureTrainClean $(OUTPUTDIR)/PaperTopicFeatureTrainClean $(OUTPUTDIR)/ConferenceTopicFeatureTrainClean $(OUTPUTDIR)/JournalTopicFeatureTrainClean $(TRAIN) $(VALID)
	$< $(LDANTOPIC) $(OUTPUTDIR)/AuthorTopicFeatureTrainClean $(OUTPUTDIR)/PaperTopicFeatureTrainClean $(OUTPUTDIR)/ConferenceTopicFeatureTrainClean $(OUTPUTDIR)/JournalTopicFeatureTrainClean $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_topic_feature_Train_clean $(OUTPUTDIR)/Valid_topic_feature_Train_clean


$(OUTPUTDIR)/Valid_topic_feature_Train_clean : $(OUTPUTDIR)/Train_topic_feature_Train_clean


$(OUTPUTDIR)/Train_caf_feature_internal_clean : $(TARGETDIR)/generate-coauthor-topic-feature $(OUTPUTDIR)/AuthorTopicFeatureInternalClean
	$< $(OUTPUTDIR)/AuthorTopicFeatureInternalClean $(PAPERAUTHORCSV) $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_caf_feature_internal_clean $(OUTPUTDIR)/Valid_caf_feature_internal_clean

$(OUTPUTDIR)/Valid_caf_feature_internal_clean : $(OUTPUTDIR)/Train_caf_feature_internal_clean

$(OUTPUTDIR)/Train_caf_feature_Train_clean : $(TARGETDIR)/generate-coauthor-topic-feature $(OUTPUTDIR)/AuthorTopicFeatureTrainClean
	$< $(OUTPUTDIR)/AuthorTopicFeatureTrainClean $(PAPERAUTHORCSV) $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_caf_feature_Train_clean $(OUTPUTDIR)/Valid_caf_feature_Train_clean

$(OUTPUTDIR)/Valid_caf_feature_Train_clean : $(OUTPUTDIR)/Train_caf_feature_Train_clean

$(OUTPUTDIR)/Train_paper_title_feature_internal_clean : $(TARGETDIR)/generate-paper-title-feature $(OUTPUTDIR)/PaperAuthor.csv.internal.Clean $(OUTPUTDIR)/paper_tokenized $(OUTPUTDIR)/paper_token_stat $(TRAIN) $(VALID)
	$< $(OUTPUTDIR)/PaperAuthor.csv.internal.Clean $(OUTPUTDIR)/paper_tokenized $(OUTPUTDIR)/paper_token_stat $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_paper_title_feature_internal_clean $(OUTPUTDIR)/Valid_paper_title_feature_internal_clean

$(OUTPUTDIR)/Valid_paper_title_feature_internal_clean  : $(OUTPUTDIR)/Train_paper_title_feature_internal_clean


$(OUTPUTDIR)/Train_paper_title_feature_Train_clean : $(TARGETDIR)/generate-paper-title-feature $(OUTPUTDIR)/PaperAuthor.csv.Train.Clean $(OUTPUTDIR)/paper_tokenized $(OUTPUTDIR)/paper_token_stat $(TRAIN) $(VALID)
	$< $(OUTPUTDIR)/PaperAuthor.csv.Train.Clean $(OUTPUTDIR)/paper_tokenized $(OUTPUTDIR)/paper_token_stat $(TRAIN) $(VALID) $(OUTPUTDIR)/Train_paper_title_feature_Train_clean $(OUTPUTDIR)/Valid_paper_title_feature_Train_clean

$(OUTPUTDIR)/Valid_paper_title_feature_Train_clean  : $(OUTPUTDIR)/Train_paper_title_feature_Train_clean

$(FEATUREDIR)/TopicFeature.Train.Train.Clean.dense : $(OUTPUTDIR)/Train_topic_feature_Train_clean $(OUTPUTDIR)/Train_caf_feature_Train_clean $(OUTPUTDIR)/Train_paper_title_feature_Train_clean 
	$(MERGER) $^ > $@

$(FEATUREDIR)/TopicFeature.Valid.Train.Clean.dense : $(OUTPUTDIR)/Valid_topic_feature_Train_clean $(OUTPUTDIR)/Valid_caf_feature_Train_clean $(OUTPUTDIR)/Valid_paper_title_feature_Train_clean 
	$(MERGER) $^ > $@

$(FEATUREDIR)/TopicFeature.Train.internal.Clean.dense : $(OUTPUTDIR)/Train_topic_feature_internal_clean $(OUTPUTDIR)/Train_caf_feature_internal_clean $(OUTPUTDIR)/Train_paper_title_feature_internal_clean 
	$(MERGER) $^ > $@

$(FEATUREDIR)/TopicFeature.Valid.internal.Clean.dense : $(OUTPUTDIR)/Valid_topic_feature_internal_clean $(OUTPUTDIR)/Valid_caf_feature_internal_clean $(OUTPUTDIR)/Valid_paper_title_feature_internal_clean 
	$(MERGER) $^ > $@

clean-feature : $(FEATUREDIR)/TopicFeature.Valid.internal.Clean.dense $(FEATUREDIR)/TopicFeature.Train.internal.Clean.dense $(FEATUREDIR)/TopicFeature.Valid.Train.Clean.dense $(FEATUREDIR)/TopicFeature.Train.Train.Clean.dense 

total-feature : $(FEATUREDIR)/Simplex.All.Train.Train.Clean.dense $(FEATUREDIR)/Simplex.All.Train.Train.Clean.sparse $(FEATUREDIR)/Simplex.All.Train.internal.Clean.dense $(FEATUREDIR)/Simplex.All.Train.internal.Clean.sparse $(FEATUREDIR)/Simplex.All.Valid.Train.Clean.dense $(FEATUREDIR)/Simplex.All.Valid.Train.Clean.sparse $(FEATUREDIR)/Simplex.All.Valid.internal.Clean.dense $(FEATUREDIR)/Simplex.All.Valid.internal.Clean.sparse 



$(FEATUREDIR)/Simplex.All.Train.%.Clean.dense : $(FEATUREDIR)/Simplex.Basic.Train.dense $(FEATUREDIR)/TopicFeature.Train.%.Clean.dense
	$(MERGER) $^ > $@

$(FEATUREDIR)/Simplex.All.Valid.%.Clean.dense : $(FEATUREDIR)/Simplex.Basic.Valid.dense $(FEATUREDIR)/TopicFeature.Valid.%.Clean.dense
	$(MERGER) $^ > $@



