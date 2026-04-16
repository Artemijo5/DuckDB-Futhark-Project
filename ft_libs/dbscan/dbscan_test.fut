import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"
import "dbscan"

module vector_2 = cat_vector vector_1 vector_1

module kd2 = kd_index vector_2 f64
module dist2 = euclidean_d vector_2 f64

module dbscan2 = ft_dbscan vector_2 f64 kd2 dist2

-- 40 pts in 4 quadrants
-- partition using kd index
-- in 1, 4, 16 partitions
-- Test clustering & collision
-- use eps=1, minPts = 2 & 3
def pts1 : [][2]f64 = [

[-5,-6],[-5.5,-6],[-5,-6.5],
[-4,-7.5],[-4.5,-7.5],
[-9,-9],
[-1.2,-5],[-0.5,-5],
[-7,-1.5],[-7,-0.6],

[0.4,-5],[0.4,-4],[0.4,-3],[0.4,-2],[0.4,-1],[0.5,-0.5],
[9,-5],
[5,-6],[5.2,-6],[5,-6.2],

[-7,0.2],[-6.2,0.2],[-6.2,1.1],[-6,1.5],[-5,1.5],[-4,1.5],[-3,1.5],[-2,1.5],[-1,1.5],[-0.5,1.7],

[0.3,1.7],[0.4,1.0],[0.5,0.4],
[2,4],[2.3,4.3],[2,4.7],[2,5.5],[2.5,5.6],[3,5.6],[3,6.3]

]

def test1 (kd : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts1) |> map (sized vector_2.length >-> vector_2.from_array)
	let (xs,parts,p_idx) = kd2.index_dataset [kd] vs
	let np = length parts
	let parts = parts |> sized np
	let p_idx = p_idx |> sized np
	let buff = dbscan2.internal_dbscan
		5 eps minPts parts p_idx xs
	in (xs, p_idx, buff.is_core, buff.chain_id)

-- eps=2, numPts=3
def pts2 : [][2]f64 = [
--[-100,-100],
[2,4],[3,4.2],[4,4.2],[2.5,5],
[9,12],
[15,24],[14,24],[14,22],
[100,100],[101,101],
--[1000,1000]
]

def test2 (kd : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts2) |> map (sized vector_2.length >-> vector_2.from_array)
	let (xs,parts,p_idx) = kd2.index_dataset [kd] vs
	let np = length parts
	let n  = length xs
	let parts = parts |> sized np
	let p_idx = p_idx |> sized np
	let buff = dbscan2.internal_dbscan
		5 eps minPts parts p_idx xs
	in (xs, p_idx, buff.is_core, buff.chain_id)