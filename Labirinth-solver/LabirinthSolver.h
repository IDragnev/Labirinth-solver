#ifndef __LAB_SOLVER_H_INCLUDED__
#define __LAB_SOLVER_H_INCLUDED__

#include "UtilityFunctions.h"
#include <vector>
#include <stack>
#include <algorithm>

namespace IDragnev
{
	template <typename Callable, typename... Args>
	void forEachArg(Callable f, Args&&... args)
	{
		(f(args), ...);
	}

	class LabirinthSolver
	{
	private:
		struct Cell;

		struct Edge
		{
			char label;
			Cell* neighbour = nullptr;
		};

		struct Cell
		{
			char symbol;
			bool isVisited = false;
			std::vector<Edge> edges;
		};

		using CellRow = std::vector<Cell>;
		using Labirinth = std::vector<CellRow>;
		using PathStack = std::stack<char>;

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
			CellRow toCellRow(InputRowRef row) const;
			
			void connectCells();
			void connectWithNeighbours(Cell& cell, std::size_t row, std::size_t column);

			template <typename... Edges>
			void addEdgeIfNotEmpty(Cell& cell, const Edges&... edges);

			auto makeSafeClear() noexcept;

		private:
			std::size_t rows = 0;
			std::size_t columns = 0;
			Labirinth result;
		};

	public:
		using Result = std::vector<std::string>;

		LabirinthSolver() = default;
		LabirinthSolver(const LabirinthSolver& source) noexcept;
		~LabirinthSolver() = default;

		LabirinthSolver& operator=(const LabirinthSolver& rhs) noexcept;

		template <typename ForwardIterator>
		Result operator()(ForwardIterator first, ForwardIterator last)
		{
			using IDragnev::Utility::CallOnDestruction;
			using LabirinthBuilder = LabirinthBuilder<ForwardIterator>;

			auto x = CallOnDestruction{ [this]() noexcept { clear(); } };
			cells = LabirinthBuilder{}(first, last);

			return findAllPaths();
		}

	private:
		Result findAllPaths();
		Cell& getStartingCellMarkingItAsVisited();
		Cell& getStartingCell();
		void startSearchFrom(Cell& c);
		void proceedSearchWith(Cell& c, const Edge& e);
		void extendCurrentPath(const Edge& e);
		void dropLatestPathExtension();
		void insertToResult(const PathStack& s);
		void clear() noexcept;

		static bool isPassable(const Cell& c) noexcept;
		static bool isTheTarget(const Cell& c) noexcept;
		static std::string makePath(PathStack s);

		template <typename Callable>
		static void forEachNeighbour(Cell& c, Callable f)
		{
			auto& edges = c.edges;
			std::for_each(std::begin(edges), std::end(edges), f);
		}

	private:
		Labirinth cells;
		PathStack currentPath;
		Result result;
	};
}

#include "LabirinthBuilderImpl.hpp"
#endif //__LAB_SOLVER_H_INCLUDED__