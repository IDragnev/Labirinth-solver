
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
		using Utility::CallOnDestruction;

		auto x = CallOnDestruction{ [this]() noexcept { clear(); } };
		init(first, last);
		buildRows(first, last);
		connectCells();

		return std::move(result);
	}

	template <typename ForwardIterator>
	void LabirinthSolver::LabirinthBuilder<ForwardIterator>::clear() noexcept
	{
		rows = columns = 0;
		result.clear();
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
		auto toCellRow = [size = columns](InputRowRef row) { return buildCellRow(row, size); };
		std::transform(first, last, std::back_inserter(result), toCellRow);
	}

	template <typename ForwardIterator>
	auto LabirinthSolver::LabirinthBuilder<ForwardIterator>::buildCellRow(InputRowRef row, std::size_t size) -> CellRow
	{
		auto result = CellRow{};
		result.reserve(size);
		auto toCell = [](char symbol) { return Cell{ symbol }; };

		std::transform(std::begin(row), std::end(row), std::back_inserter(result), toCell);

		return result;
	}

	template <typename ForwardIterator>
	void LabirinthSolver::LabirinthBuilder<ForwardIterator>::connectCells()
	{
		for (decltype(rows) i = 0; i < rows; ++i)
		{
			for (decltype(columns) j = 0; j < columns; ++j)
			{
				connectWithNeighbours(result[i][j], i, j);
			}
		}
	}

	template <typename ForwardIterator>
	void LabirinthSolver::LabirinthBuilder<ForwardIterator>::connectWithNeighbours(Cell& cell, std::size_t row, std::size_t column)
	{
		auto west = Edge{ 'W', (column > 0u) ? &result[row][column - 1u] : nullptr };
		auto east = Edge{ 'E', (column + 1u < columns) ? &result[row][column + 1u] : nullptr };
		auto north = Edge{ 'N', (row > 0u) ? &result[row - 1u][column] : nullptr };
		auto south = Edge{ 'S', (row + 1u < rows) ? &result[row + 1u][column] : nullptr };

		addEdgeIfNotEmpty(cell, west, east, north, south);
	}

	template <typename ForwardIterator>
	template <typename... Edges>
	void LabirinthSolver::LabirinthBuilder<ForwardIterator>::addEdgeIfNotEmpty(Cell& cell, const Edges&... edges)
	{
		auto insert = [&cell](const auto& edge) 
		{
			if (edge.neighbour != nullptr)
			{
				cell.edges.push_back(edge);
			}
		};

		forEachArg(insert, edges...);
	}
}