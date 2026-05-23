# Gigiquant — Assignment Explanation

Gigiquant is a C project that analyzes stock market data using several different approaches, from basic statistics all the way to probability modeling. Each group of tests tackles a different problem, and the code is split across modules accordingly.

You can find the full source on GitHub: [github.com/paun-cristian/Gigiquant](https://github.com/paun-cristian/Gigiquant)

---

## Task 1: Reading and Analyzing Stock Data (`data.c`)

This is where raw price data gets loaded and turned into something meaningful.

The program reads a list of daily stock prices, then computes three standard financial metrics:

- **Average return** — the mean of the day-to-day percentage changes, which tells you the general direction the stock has been moving.
- **Volatility** — how wildly those returns swing up and down (standard deviation). A high volatility means the price is hard to predict; a low one means it's been fairly stable.
- **Sharpe ratio** — dividing the average return by the volatility gives you a sense of how much reward you're getting per unit of risk. A higher ratio is generally better.

All three are truncated (not rounded) to three decimal places before being written to the output file.

---

## Task 2: Finding Trading Opportunities (`data.c`)

This task looks for arbitrage — situations where the same asset is priced differently in different places, creating a risk-free profit opportunity.

The input contains daily prices from three different cities. For each day, the program checks whether two cities have the same price while the third is different. When that happens, it reports the day, the price gap, and which city is the outlier.

The tricky part is that the data for each city is stored as a stack (most recent price on top), so the program has to read through all three stacks in sync. It stops as soon as the shortest stack runs out.

---

## Task 3: Tracking Stock Movements with a Tree (`tree.c`)

This task organizes how multiple stocks move over time into a binary tree, then finds pairs of stocks that always moved in opposite directions.

Each stock travels through the tree day by day: it goes right when its price rises and left when it falls. After all stocks have been placed, the program looks for stocks that ended up in the same leaf node — meaning they took completely mirrored paths through the tree. Those are the pairs that always moved in opposite directions relative to each other.

The result is a list of matched pairs, printed without duplicates.

---

## Task 4: Predicting Prices with Markov Chains (`markov.c`)

This is the most mathematically involved part of the project. The goal is to estimate the probability of a stock reaching a target price within a given number of days, based purely on historical movement patterns.

Here's how it works:

1. **Bucketing prices:** Prices are grouped into intervals (e.g. every 0.5 units). This turns a continuous price history into a finite set of states.
2. **Building the graph:** Each time the historical data shows a price moving from one bucket to another, an edge is added between those two nodes. The result is a directed graph of transitions.
3. **Compressing the indices:** Only buckets that actually appear in the data are used. This keeps memory usage tight regardless of how spread out the prices are.
4. **Simulating day by day:** Starting from the first observed price (probability = 1), the program distributes probability across the graph at each step. Each outgoing edge from a node gets an equal share of that node's current probability.
5. **Exact fractions:** All probabilities are stored as fractions (numerator/denominator) and simplified using the GCD at every step. This avoids the rounding errors that floating-point arithmetic would introduce over many iterations.

The probability of being at the target price is printed after each day, expressed as a simplified fraction like `3/8`, or just a whole number if the denominator is 1.

---

## How to Contribute

Contributions are welcome! Here's how to get involved:

1. **Fork the repository** on GitHub and clone your fork locally.
2. **Create a branch** for your changes — something descriptive like `fix-memory-leak` or `add-test-coverage`.
3. **Make your changes**, keeping the existing code style in mind (C99, manual memory management, no external dependencies).
4. **Test your changes** against the existing test inputs before submitting.
5. **Open a pull request** on the main repo with a clear description of what you changed and why.

If you find a bug or have an idea for an improvement, feel free to open an issue first to discuss it before writing code. The issue tracker is at [github.com/paun-cristian/Gigiquant/issues](https://github.com/paun-cristian/Gigiquant/issues).
