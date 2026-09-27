// DBMS_C demo script
// Copy statements one by one into NewDBMS.exe.
// Default database: Show2

CREATE TABLE student(id INT, name VARCHAR(20), age INT);
INSERT INTO student(id, name, age) VALUES(1, 'wang', 22);
INSERT INTO student(id, name, age) VALUES(2, 'li', 21);
INSERT INTO student(id, name, age) VALUES(3, 'zhang', 23);

SELECT id, name, age FROM student;
SELECT id, name FROM student WHERE age > 21;

CREATE TABLE score(sid INT, course VARCHAR(20), grade INT);
INSERT INTO score(sid, course, grade) VALUES(1, 'db', 90);
INSERT INTO score(sid, course, grade) VALUES(2, 'db', 85);

SELECT name, course, grade FROM student, score WHERE id = sid;

UPDATE student SET age = 24 WHERE id = 1;
SELECT id, name, age FROM student WHERE id = 1;

DELETE FROM student WHERE id = 3;
SELECT id, name, age FROM student;

CREATE VIEW student_view AS SELECT id, name FROM student;
SELECT id, name FROM student_view;

CREATE INDEX idx_student_id ON student(id);

commit;
