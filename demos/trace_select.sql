trace on;

CREATE TABLE student(id INT, name VARCHAR(20));
INSERT INTO student(id, name) VALUES(1, 'alice');
INSERT INTO student(id, name) VALUES(2, 'bob');

SELECT id, name FROM student WHERE id=1;
