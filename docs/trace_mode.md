# Trace Mode

Trace mode is a CLI teaching visualization aid. It shows how SQL moves through the internal path from parsing, to plan generation, to scan execution, to result output.

Trace is disabled by default, so normal database output is unchanged unless tracing is enabled.

## Enable Trace

Start the program with Trace enabled:

```powershell
bin\NewDBMS.exe --trace
```

Or enable and disable tracing inside the SQL prompt:

```sql
trace on;
trace off;
```

You can check the current state with:

```sql
trace;
```

## Supported Scope

The current MVP supports the main `SELECT` query path:

```text
QueryData -> Plan tree -> Scan chain -> Output
```

## Example SQL

```sql
trace on;
CREATE TABLE student(id INT, name VARCHAR(20));
INSERT INTO student(id, name) VALUES(1, 'alice');
INSERT INTO student(id, name) VALUES(2, 'bob');
SELECT id, name FROM student WHERE id=1;
```

## Example Output

The exact result table depends on the current database contents, but Trace lines use this format:

```text
[TRACE_PARSE] QueryData
[TRACE_PARSE]   fields: id, name
[TRACE_PARSE]   tables: student
[TRACE_PARSE]   predicate: id=1
[TRACE_PLAN] Plan tree
[TRACE_PLAN]   ProjectPlan fields=id, name
[TRACE_PLAN]     SelectPlan predicate=id=1
[TRACE_PLAN]       TablePlan table=student
[TRACE_SCAN] Scan chain
[TRACE_SCAN]   ProjectScan fields=id, name
[TRACE_SCAN]     SelectScan
[TRACE_SCAN]       TableScan file=student.tbl
[TRACE_OUTPUT] Rendering SELECT result rows
[TRACE_OUTPUT] result rows: 1
```

## Current Limitations

- The first version mainly covers the `SELECT` main path.
- `CREATE`, `INSERT`, `UPDATE`, `DELETE`, `COMMIT`, and `ROLLBACK` Trace output will be added gradually by command type.
- This is CLI teaching visualization, not a GUI.
- Trace is disabled by default and does not affect normal execution output.
- `exit` follows the existing shutdown path and commits the current transaction, so Trace mode may show a final `COMMIT` trace while the database is closing.
