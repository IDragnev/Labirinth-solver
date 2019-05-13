#include "LabirinthSolver.h"

namespace IDragnev
{
	//all members act like local variables
	//and need not be copied
	LabirinthSolver::LabirinthSolver(const LabirinthSolver&) noexcept
	{
	}

	LabirinthSolver& LabirinthSolver::operator=(const LabirinthSolver&) noexcept
	{
		return *this;
	}

	auto LabirinthSolver::findAllPaths() -> Result
	{
		auto& start = getStartingCellMarkingItAsVisited();

		startSearchFrom(start);

		return std::move(result);
	}

	auto LabirinthSolver::getStartingCellMarkingItAsVisited() -> Cell&
	{
		auto& start = getStartingCell();
		start.isVisited = true;

		return start;
	}

	auto LabirinthSolver::getStartingCell() -> Cell&
	{
		for (auto& row : cells)
		{
			for (auto& cell : row)
			{
				if (cell.symbol == 's')
				{
					return cell;
				}
			}
		}

		throw std::invalid_argument{ "No starting cell" };
	}

	void LabirinthSolver::startSearchFrom(Cell& cell)
	{
		forEachNeighbour(cell, [this](auto& edge)
		{
			auto& neighbour = *edge.neighbour;

			if (!neighbour.isVisited && isPassable(neighbour))
			{
				proceedSearchWith(neighbour, edge);
			}
		});
	}

	bool LabirinthSolver::isPassable(const Cell& c) noexcept
	{
		return c.symbol != '*';
	}

	void LabirinthSolver::proceedSearchWith(Cell& cell, const Edge& edge)
	{
		using IDragnev::Utility::CallOnDestruction;

		auto safeLeave = [&cell, this]() noexcept { cell.isVisited = false; dropLatestPathExtension(); };
		auto x = CallOnDestruction{ safeLeave };

		cell.isVisited = true;
		extendCurrentPath(edge);

		if (isTheTarget(cell))
		{
			insertToResult(currentPath);
		}
		else
		{
			startSearchFrom(cell);
		}
	}

	void LabirinthSolver::extendCurrentPath(const Edge& e)
	{
		currentPath.push(e.label);
	}

	bool LabirinthSolver::isTheTarget(const Cell& c) noexcept
	{
		return c.symbol == 'e';
	}

	void LabirinthSolver::insertToResult(const PathStack& s)
	{
		result.push_back(makePath(s));
	}

	std::string LabirinthSolver::makePath(PathStack stack)
	{
		auto length = stack.size();
		auto result = std::string(length, ' ');

		for (decltype(auto) i = length; i > 0; --i)
		{
			result[i - 1] = stack.top();
			stack.pop();
		}

		return result;
	}

	void LabirinthSolver::dropLatestPathExtension()
	{
		currentPath.pop();
	}

	void LabirinthSolver::clear() noexcept
	{
		result.clear();
		cells.clear();		
		currentPath = {};
	}
}