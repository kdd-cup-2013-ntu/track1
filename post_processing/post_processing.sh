g++ post_processing.cpp -o post_processing
g++ make_post.cpp -o make_post
cat ${2} | cut -d " " -f 15 | cut -d ":" -f 2 > Valid.dup
./make_post ${1}
./post_processing ${3} ${4}
