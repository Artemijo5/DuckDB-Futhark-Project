-- Module for Z-order computations

import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/sorts/radix_sort"

-- Performs z-order on vectors of positive i64.
module z_order (V : vector) = {
	type vector 'a = V.vector a

	def interleave
		(vec : V.vector i64)
	: V.vector i64 =
		if false then
		let msb = vec |> V.map (i64.clz) |> V.map ((i32.-) i64.num_bits)
			|> V.reduce (i32.max) (-1)
			|> i64.i32
		in loop vec1 = V.replicate 0i64
		for j<(1+msb)*V.length do
			let pos_from = j%V.length
			let pos_to = j/V.length
			let bit_from = i32.i64 (j/msb)
			let bit_to = i32.i64 (j%msb)
			let got_bit = V.get pos_from vec
				|> i64.get_bit bit_from
			let with_bit_set = i64.set_bit bit_to
				(V.get pos_to vec1) got_bit
			in V.set pos_to with_bit_set vec1
		else vec

	def order_by_z_curve [n] 't
		(z_vecs : [n](V.vector i64))
	: ([n](V.vector i64), [n]i64) =
		loop (vecs,is)
			= (z_vecs, iota n)
		for j<V.length do
			let cur_dim = vecs |> map (V.get j)
			let msb = cur_dim |> map (i64.clz) |> map ((i32.-) i64.num_bits)
				|> i32.maximum
			let (_,vecs1,is1) = radix_sort_by_key (.0) msb (i64.get_bit) (zip3 cur_dim vecs is)
				|> unzip3
			in (vecs1, is1)
			
}
