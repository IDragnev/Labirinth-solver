#ifndef __LAB_SOLVER_H_INCLUDED__
#define __LAB_SOLVER_H_INCLUDED__

#include <vector>
#include <array>

namespace IDragnev
{
	class LabirinthSolver
	{
	private:
		struct Cell;

		struct Edge
		{
			char label;
			Cell* neighbour = nullptr;
		};

		static constexpr std::size_t MAX_NEIGHBOURS_COUNT = 4;

		struct Cell
		{
			char symbol;
			bool isVisited = false;
			std::array<Edge, MAX_NEIGHBOURS_COUNT> edges;
		};

		using CellRow = std::vector<Cell>;
		using Labirinth = std::vector<CellRow>;

		template <typename ForwardIterator>
		class LabirinthBuilder
		{
			using InputRow = typename std::iterator_traits<ForwardIterator>::value_type;
			using InputRowRef = std::conditional_t<std::is_pointer_v<InputRow>, InputRow, const InputRow&>;

		public:
			LabirinthBuilder() = default;
			LabirinthBuilder(const LabirinthBuilder& source) noexcept;
			~LabirinthBuilder() = default;

			LabirinthBuilder& operator=(const LabirinthBuilder& rhs) noexcept;

			Labirinth operator()(ForwardIterator first, ForwardIterator last);

		private:
			void init(ForwardIterator first, ForwardIterator last);
			void setDimensions(ForwardIterator first, ForwardIterator last);
			void reserveMemory();
			
			void buildRows(ForwardIterator first, ForwardIterator last);
			static CellRow toCellRow(InputRowRef row);
			
			void connectCells();
			void connectWithNeighbours(Cell& cell, std::size_t row, std::size_t column);

			auto makeSafeClear() noexcept;

		private:
			std::size_t rows = 0;
			std::size_t columns = 0;
			Labirinth result;
		};

	public:


	private:
		Labirinth cells;
	};
}

#include "LabirinthBuilderImpl.hpp"
#endif //__LAB_SOLVER_H_INCLUDED__