
namespace IDragnev
{
	//all members act like local variables
	//and need not be copied
	template <typename ForwardIterator>
	LabirinthSolver::LabirinthBuilder<ForwardIterator>::LabirinthBuilder(const LabirinthBuilder&) noexcept
	{
	}

	template <typename ForwardIterator>
	inline auto LabirinthSolver::LabirinthBuilder<ForwardIterator>::operator=(const LabirinthBuilder&) noexcept -> LabirinthBuilder&
	{
		return *this;
	}

	template <typename ForwardIterator>
	auto LabirinthSolver::LabirinthBuilder<ForwardIterator>::operator()(ForwardIterator first, ForwardIterator last) -> Labirinth
	{
		auto clear = makeSafeClear();
		init(first, last);
		buildRows(first, last);
		connectCells();

		return std::move(result);
	}

	template <typename ForwardIterator>
	auto LabirinthSolver::LabirinthBuilder<ForwardIterator>::makeSafeClear() noexcept
	{
		auto deleter = [](auto ptr) noexcept
		{		
			ptr->rows = ptr->columns = 0;
	   		ptr->result.clear();
		};

		using ScopedClear = std::unique_ptr<LabirinthBuilder, decltype(deleter)>;
		return ScopedClear{ this, deleter };
	}

	template <typename ForwardIterator>
	inline void LabirinthSolver::LabirinthBuilder<ForwardIterator>::init(ForwardIterator first, ForwardIterator last)
	{
		setDimensions(first, last);
		reserveMemory();
	}

	template <typename ForwardIterator>
	void LabirinthSolver::LabirinthBuilder<ForwardIterator>::setDimensions(ForwardIterator first, ForwardIterator last)
	{
		decltype(auto) firstRow = *first;

		rows = std::distance(first, last);
		columns = std::distance(std::begin(firstRow), std::end(firstRow));
	}

	template <typename ForwardIterator>
	inline void LabirinthSolver::LabirinthBuilder<ForwardIterator>::reserveMemory()
	{
		result.reserve(rows);
	}

	template <typename ForwardIterator>
	inline void LabirinthSolver::LabirinthBuilder<ForwardIterator>::buildRows(ForwardIterator first, ForwardIterator last)
	{
		std::transform(first, last, std::back_inserter(result), toCellRow);
	}

	template <typename ForwardIterator>
	auto LabirinthSolver::LabirinthBuilder<ForwardIterator>::toCellRow(InputRowRef row) -> CellRow
	{
		auto result = CellRow{};
		result.reserve(columns);
		auto toCell = [](char symbol) { return Cell{ symbol }; };

		std::transform(std::begin(row), std::end(row), std::back_inserter(result), toCell);

		return result;
	}

	template <typename ForwardIterator>
	void LabirinthSolver::LabirinthBuilder<ForwardIterator>::connectCells()
	{
		for (decltype(rows) row = 0; row < rows; ++row)
		{
			for (decltype(columns) col = 0; col < columns; ++col)
			{
				connectWithNeighbours(cells[row][col], row, col);
			}
		}
	}
}