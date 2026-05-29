import "lib/github.com/diku-dk/sorts/radix_sort"
import "tp_sort"

module keyTps = mk_keyTps_numeric i64

let srcs : [][]i64 = [
	[7,9,10,12,52,57],
	[1,5,6,6,12,13],
	[0,0,2,4,9,9],
	[2,3,3,8,13,19],
	[4,5,5,9,16,16],
	[7,7,8,9,11,13],
	[1,3,5,7,8,10]
]

--def sort1 (xss : [][]i64) = radix_sort_int i64.num_bits i64.get_bit (xss |> flatten)

-- TODO this currently refuses to work in the repl due to array size issues
-- of course I mainly need it as a model for the C pipeline rather than to use it here
-- so low priority...

entry test_tps (cnk_sz : i64) =
	-- Init parameters
	let srcs_No = length srcs
	let cnks_No = srcs_No + 1
	let src_sz = length (head srcs)
	let src_cnks = (src_sz + cnk_sz - 1) / cnk_sz
	-- make payload data
	let pL = iota srcs_No
		|> map (\i -> iota src_sz |> map (\j -> i*src_sz + j))
	-- Init processor
	let buff_info = init_bufferInfo cnk_sz cnks_No srcs_No
	let buff_proc0 = init_bufferProc buff_info
	-- Init key & pL buffers
	let ks_buffs0 = keyTps.init_ks_buffer buff_info
	let pL_buffs0 = init_pL_buffer 0i64 buff_info
	let ks_waiting0 = keyTps.init_ks_waiting buff_info
	let pL_waiting0 = init_pL_waiting 0i64 buff_info
	-- Load first chunks to waiting buffers
	let (ks_waiting1, pL_waiting1, buff_proc1)
	= loop (ks_waiting, pL_waiting, buff_proc) = (ks_waiting0, pL_waiting0, copy buff_proc0)
		for j<srcs_No do
			let toLoad = i64.min cnk_sz src_sz
			let new_ks = keyTps.writeToWaiting_ks j (copy srcs[j][0:toLoad]) ks_waiting
			let new_pL = writeToWaiting_pL j buff_info (copy pL[j][0:toLoad]) pL_waiting
			let new_proc = writeToWaiting_proc j toLoad buff_info (copy buff_proc)
			in (new_ks, new_pL, new_proc)
	let init_src_cursors = replicate srcs_No (i64.min cnk_sz src_sz)
	-- Initialize dest buffs
	let init_dest_ks = replicate (srcs_No * src_sz) (-1i64)
	let init_dest_pL = replicate (srcs_No * src_sz) (-1i64)
	-- Nested loop for 2-Pass Sort
	let n_buff = length ks_buffs0
	let ks_buffs0_0 = ks_buffs0 |> sized n_buff
	let pL_buffs0_0 = pL_buffs0 |> sized n_buff
	let (final_dest_ks, final_dest_pL, _, _, _, _, _, _, _)
	= loop (dest_ks, dest_pL, ks_buffs, pL_buffs, ks_waiting, pL_waiting, buff_proc, src_cursors, dest_cursor)
	= (init_dest_ks, init_dest_pL, ks_buffs0_0, pL_buffs0_0, ks_waiting1, pL_waiting1, copy buff_proc1, init_src_cursors, 0)
	while !(all (id) buff_proc.isSrcExhausted) do
		-- Inner Loop
		let (in_ks_buffs, in_pL_buffs, in_ks_waiting, in_pL_waiting, in_buff_proc, in_src_cursors)
		= loop (inn_ks_buffs, inn_pL_buffs, inn_ks_waiting, inn_pL_waiting, inn_buff_proc, inn_src_cursors)
		= (ks_buffs, pL_buffs, ks_waiting, pL_waiting, buff_proc, src_cursors)
		while !(all (id) inn_buff_proc.isChunkTaken) do
			let next_src = keyTps.next_waiting inn_buff_proc inn_ks_waiting
			let next_cnk = next_unusedChunk inn_buff_proc
			let new_ks_buffs = keyTps.writeToBuffer_ks next_src next_cnk inn_ks_waiting inn_ks_buffs
			let new_pL_buffs = writeToBuffer_pL next_src next_cnk buff_info inn_pL_waiting inn_pL_buffs
			let new_proc = writeToBuffer_proc next_src next_cnk (copy inn_buff_proc)
			-- Update waiting buffer (or mark exhausted)
			let offs = inn_src_cursors[next_src]
			let limt = i64.min cnk_sz (src_sz - offs)
			let new_ks_waiting = keyTps.writeToWaiting_ks next_src (copy srcs[next_src][offs:limt]) inn_ks_waiting
			let new_pL_waiting = writeToWaiting_pL next_src buff_info (copy pL[next_src][offs:limt]) inn_pL_waiting
			let new_proc' = if limt>0
				then writeToWaiting_proc next_src limt buff_info (copy new_proc)
				else markSrcExhausted next_src (copy new_proc)
			let new_src_cursors = (copy inn_src_cursors) with [next_src] = offs+limt
			in (new_ks_buffs, new_pL_buffs, new_ks_waiting, new_pL_waiting, new_proc', new_src_cursors)
		-- Sort
		let (sorted_ks_buff, sorted_pL_buff) = radix_sort_int
			i64.num_bits
			(\b (k,_)-> i64.get_bit b k)
			(zip in_ks_buffs in_pL_buffs)
		|> sized n_buff
		|> unzip
		-- Fetch
		let fetched_ks = keyTps.fetchSorted_ks buff_info in_buff_proc sorted_ks_buff
		let fetched_pL = fetchSorted_pL buff_info in_buff_proc
			(sorted_pL_buff |> sized (buff_info.chunks_No * buff_info.chunkSize))
		let sorted_ks_buff' = keyTps.afterFetching_ks buff_info in_buff_proc (copy sorted_ks_buff)
		let new_proc = fetchSorted_proc buff_info (copy in_buff_proc)
		-- Append to dest arrays
		let new_dest_ks = (copy dest_ks) with [dest_cursor:dest_cursor+(length fetched_ks)] = fetched_ks
		let new_dest_pL = (copy dest_pL) with [dest_cursor:dest_cursor+(length fetched_pL)] = fetched_pL
		let new_dest_cursor = dest_cursor+(length fetched_ks)
		in (
			new_dest_ks, new_dest_pL,
			sorted_ks_buff' |> sized n_buff, sorted_pL_buff |> sized n_buff,
			in_ks_waiting, in_pL_waiting,
			new_proc,
			in_src_cursors, new_dest_cursor
		)
	in (final_dest_ks, final_dest_pL)




