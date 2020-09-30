# JSON_Parser
A C++ Parser that I compile to a Python extension library. Used to convert a JSON structure to a relational structure (e.g., Pandas) much faster than pure python

## Motivation
For my work, I download a database of user data for a gaming app every 24 hours. I use this data to detect any unusual acitivity (i.e., hacking) by looking for outliers. However, the database is NoSQL and in a JSON format, which cannot be directly imported into a pandas DataFrame for easy manipulating. This meant that I needed to do multiple dictionary comprehensions to flatten the data to a relational structure. I also needed to process some of the data to calculate values that were not stored in the database. This came down to an multiple for loops with each going through the hundreds of thousands of accounts. The process took a long time.

## Solution
This library performs all of the same tasks from the original pure python script, but it instead parses the downloaded JSON string into a relational structure one user at a time. The resulting runtime for this code is significantly reduced, allowing me to download data for four app databases, parse the data, and detect outliers in much less time. Whereas before the process required 40 minutes, it can all be completed in about 10 minutes now.
