import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"
import "dbscan"
import "ft_dclust"
import "ft_densebox"
-- import "ft_dclust_alt"
--import "ft_dclust_single"

module vector_2 = cat_vector vector_1 vector_1

module dist2 = euclidean_d vector_2 f64

module dbscan2 = ft_dbscan vector_2 f64 dist2
module dclust2 = ft_dclust vector_2 f64 dist2
module dnsbox2 = ft_densebox vector_2 f64 dist2

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

def test1 (extPar : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts1) |> map (sized vector_2.length >-> vector_2.from_array)
	let (is_core, chain_id) = dbscan2.internal_dbscan extPar eps minPts vs
	in  (vs, is_core, chain_id)

def test1_dclust (seed_count : i64) (subdiv : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts1) |> map (sized vector_2.length >-> vector_2.from_array)
	let sdv = replicate vector_2.length subdiv
	in dclust2.do_dclust 10000 seed_count sdv eps minPts vs

def test1_dnsbox (eps : f64) (minPts : i64) =
	let vs = (copy pts1) |> map (sized vector_2.length >-> vector_2.from_array)
	in dnsbox2.do_dbscan 5 eps minPts vs

-- eps=2, minPts=3
def pts2 : [][2]f64 = [
--[-100,-100],
[2,4],[3,4.2],[4,4.2],[2.5,5],
[9,12],
[15,24],[14,24],[14,22],
--[100,100],[101,101],
--[1000,1000]
]

def test2 (extPar : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts2) |> map (sized vector_2.length >-> vector_2.from_array)
	let (is_core, chain_id) = dbscan2.internal_dbscan extPar eps minPts vs
	in  (vs, is_core, chain_id)

def test2_dclust (seed_count : i64) (subdiv : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts2) |> map (sized vector_2.length >-> vector_2.from_array)
	let sdv = replicate vector_2.length subdiv
	in dclust2.do_dclust 10000 seed_count sdv eps minPts vs

def test2_dnsbox (eps : f64) (minPts : i64) =
	let vs = (copy pts2) |> map (sized vector_2.length >-> vector_2.from_array)
	in dnsbox2.do_dbscan 5 eps minPts vs

def pts3 : [][2]f64 = [
	[4.482,8.931],
	[1.020,2.004],
	[1.582,2.900],
	[1.057,2.404],
	[7.728,5.220],
	[4.382,9.081],
	[1.461,1.890],
	[1.272,2.622],
	[4.128,8.856],
	[7.900,5.010]
]

def test3 (extPar : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts3) |> map (sized vector_2.length >-> vector_2.from_array)
	let (is_core, chain_id) = dbscan2.internal_dbscan extPar eps minPts vs
	in  (vs, is_core, chain_id)

def test3_dclust (seed_count : i64) (subdiv : i64) (eps : f64) (minPts : i64) =
	let vs = (copy pts3) |> map (sized vector_2.length >-> vector_2.from_array)
	let sdv = replicate vector_2.length subdiv
	in dclust2.do_dclust 10000 seed_count sdv eps minPts vs

def test3_dnsbox (eps : f64) (minPts : i64) =
	let vs = (copy pts3) |> map (sized vector_2.length >-> vector_2.from_array)
	in dnsbox2.do_dbscan 5 eps minPts vs


def pts4 : [][2]f64 = [
	[6,0],
	[0,15],
	[14.4,2],[13.6,2],[14,1.6],[14,2],[14.3,2.6],[13.7,2.6],[14,2.87],[14,2.4],
	[10.1,9.8],[15,8],
	[4.8,4.9],[4.9,4.8],
	[5.1,5.1],[5.55,5.1],[5.1,5.55],[5.8,5.2],[9.6,9.9],
	[1,9.9],[0.6,9.9],[1.2,9.7],[0.8,9.7],
	[1,10.35],
	[9.8,10.5],[9.8,10.1],
	[10.2,10.2]
]

def test4 =
	let vs = (copy pts4) |> map (sized vector_2.length >-> vector_2.from_array)
	let (is_core, chain_id) = dbscan2.internal_dbscan 5 0.5 5 vs
	in  (vs, is_core, chain_id)

def test4_dclust =
	let vs = (copy pts4) |> map (sized vector_2.length >-> vector_2.from_array)
	let sdv = replicate vector_2.length 3
	in dclust2.do_dclust 10000 4 sdv 0.5 5 vs

def test4_dnsbox =
	let vs = (copy pts4) |> map (sized vector_2.length >-> vector_2.from_array)
	let sdv = replicate vector_2.length 3
	in dnsbox2.do_dbscan 5 0.5 5 vs