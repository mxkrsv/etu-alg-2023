#!/usr/bin/env sh

set -e

VALGRIND_ARGS="--quiet --tool=massif --stacks=yes --threshold=0 --peak-inaccuracy=0 \
	--detailed-freq=1 --max-snapshots=1000 --ignore-fn=_GLOBAL__sub_I_eh_alloc.cc"

msizes="$2"
pows="$1"

bench_single() {
	valgrind $VALGRIND_ARGS --massif-out-file=massif.out.tmp ./main "$1" "$2"
	./massif_peak_usage/massif_peak_usage massif.out.tmp
	rm massif.out.tmp
}

for pow in $pows; do
	for msize in $msizes; do
		echo "Testing with pow=$pow and msize=$msize"
		bench_single "$pow" "$msize"
	done
done
