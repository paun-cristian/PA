# Gigiquant - Assignment Explanation

This project implements various data structures and algorithms to analyze stock market trends, calculate moving averages, find trading opportunities, and predict price changes using statistical models.

Here is a simple breakdown of what each task does:

## Task 1: Stock Market Data Reading (`data.c`)

This section focuses on loading the raw stock data from the input files into memory.

- It reads multiple lines where each line represents a different stock ticker (a company's stock).
- It parses the data step by step, splitting out the varying prices on different days.
- It calculates simple statistics, like the **Simple Moving Average (SMA)**, which is just the average price of a stock over the last few days. This helps "smooth out" the price fluctuations to see the actual trend.

## Task 2: Finding Trading Opportunities (`data.c`)

This task looks at the stock data we loaded and tries to find the best moments to buy or sell.

- It iterates through the stock history and tracks price momentum (whether stock prices are consistently going up or down).
- It looks for "crossovers" — specific moments where short-term price trends cross above or below long-term price trends. In trading, these crossovers are major signals that a stock is about to jump up or drop down.
- Based on these signals, it outputs recommendations on what action to take (e.g., Buy, Sell, or Hold).

## Task 3: Tree Indexing (`tree.c`)

This task organizes the trading information into a special hierarchical structure called a **Tree**.

- Instead of keeping data in a flat, messy list, we take the trading actions and organize them as nodes in a tree.
- When new trading actions or prices come in, they are inserted into the tree based on specific rules (like how price intervals are structured).
- Using a tree makes it significantly faster to search for historical prices, find specific price ranges, or update data, just like how a library's index helps you find a book much faster than checking every shelf.

## Task 4: Prediction Using Markov Chains (`markov.c`)

The final and most complex task tries to predict the probability of a stock reaching a specific target price using **Markov Chains**.

- A Markov Chain is a mathematical system that transitions from one state (price interval) to another based purely on probabilities drawn from historical data.
- **Coordinate Compression:** Because stock prices might jump from 100 to 200, assigning a massive array of 200 slots wastes memory. Instead, we use "coordinate compression" to map these sparse prices cleanly to indices: 0, 1, 2.
- **Graph Building:** We treat every price interval as a "node." If historical data shows a stock went from 100 to 105, we draw an arrow (edge) between them. If it went from 100 to 105 ten times, that path becomes highly probable.
- **Simulating the Future:** To find the chance of reaching our `target` price within a few `days`, we run a simulation (using Breadth-First Search). We start at day 0 with a $100\%$ chance of being at the `start` price. As the days tick by, we distribute those probabilities down the connected arrows based on history.
- **Exact Math:** Everything is calculated using strictly simplified fractions (like $11/18$), utilizing the Greatest Common Divisor (GCD) to ensure mathematical precision without rounding errors.
