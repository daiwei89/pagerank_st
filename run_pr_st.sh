
#data_file=datasets/barabasi-90000.txt
#data_file=datasets/twitter_rv_small_1e7.net
data_file=datasets/twitter_rv.net
num_iterations=50


GLOG_logtostderr=true \
GLOG_v=-1 \
GLOG_minloglevel=0 \
./bin/pagerank_main \
 --data_file $data_file \
 --num_iterations $num_iterations \
