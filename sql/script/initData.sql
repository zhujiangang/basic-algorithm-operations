CREATE DATABASE mytest
ON 
(  NAME = mytest_dat,
   FILENAME = 'C:\lgao1\81sql\data\mytestdat.mdf',
   SIZE = 10,
   MAXSIZE = 50,
   FILEGROWTH = 5 )
LOG ON
( NAME = 'mytest_log',
   FILENAME = 'C:\lgao1\81sql\data\mytestlog.ldf',
   SIZE = 5MB,
   MAXSIZE = 25MB,
   FILEGROWTH = 5MB )

create table Student
(
  S# int PRIMARY KEY,
  Sname varchar(30),
  Sage tinyint NOT NULL
    DEFAULT 0,
  Ssex tinyint NOT NULL,
)

create table Teacher
(
  T# int PRIMARY KEY,
  Tname varchar(30),
)

create table Course
(
  C# int PRIMARY KEY,
  Cname varchar(30),
  T# int NOT NULL
    FOREIGN KEY (T#) REFERENCES Teacher(T#)
)

create table SC
(
   S# int,
   C# int,
   score int NOT NULL
     DEFAULT 0,
   PRIMARY KEY (S#,C#) 
)

insert into Student values(1, 'Jim',21,0)
insert into Student values(2, 'Lucy',20,1)
insert into Student values(3, 'Colin',21,0)
insert into Student values(4, 'Mary',22,1)
insert into Student values(5, 'Tom',19,0)
insert into Student values(6, 'Jack',31,0)
insert into Student values(7, 'Berry',22,1)
insert into Student values(8, 'Julia',21,1)
select * from Student

insert into Teacher values(1, 'Mr Yang')
insert into Teacher values(2, 'Ms Yue')
insert into Teacher values(3, 'Ian')
insert into Teacher values(4, 'Dracon')
update Teacher set Tname='Yang HL' where T#=1
update Teacher set Tname='Yue XY' where T#=2
update Teacher set Tname='Li WF' where T#=3
update Teacher set Tname='Li WF' where T#=4
select * from Teacher


insert into Course values(001,'CPP Programming',3)
insert into Course values(002,'Data Structure',4)
insert into Course values(003,'English',1)
insert into Course values(004,'English',2)
insert into Course values(005,'Data Structure',3)
insert into Course values(006,'OS',1)
select * from Course

insert into SC values(1, 003, 99)
insert into SC values(1, 001, 66)
insert into SC values(1, 002, 33)
insert into SC values(2, 003, 90)
insert into SC values(2, 006, 68)
insert into SC values(3, 004, 80)
insert into SC values(3, 002, 50)
insert into SC values(4, 006, 88)
insert into SC values(4, 005, 90)
insert into SC values(5, 004, 73)
insert into SC values(5, 006, 86)
insert into SC values(5, 105, 90)
insert into SC values(6, 006, 77)
insert into SC values(6, 002, 84)
insert into SC values(6, 001, 69)
insert into SC values(7, 003, 78)
insert into SC values(7, 001, 3)
insert into SC values(7, 002, 55)
insert into SC values(7, 004, 44)
insert into SC values(8, 001, 66)
insert into SC values(8, 002, 79)
select * from SC


delete from Teacher
delete from Course
delete from SC

select * from Student
select * from Teacher
select * from Course
select * from SC