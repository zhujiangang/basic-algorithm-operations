
/**
Student(S#,Sname,Sage,Ssex) 学生表 
Course(C#,Cname,T#) 课程表 
SC(S#,C#,score) 成绩表 
Teacher(T#,Tname) 教师表 
**/

select * from Student
select * from Course
select * from SC
select * from Teacher

--1、查询“001”课程比“002”课程成绩高的所有学生的学号； 
select a.S#, c.Sname from 
(select S#,score from SC where C#=001) a, 
(select S#,score from SC where C#=002) b, 
Student c 
where a.score > b.score and a.S#=b.S# and c.S#=a.S#

--2、查询平均成绩大于60分的同学的学号和平均成绩； 
select S#,avg(score) from SC group by S# having avg(score) > 60
select a.Sname, b.S#, b.avgs from Student a, (select S#,avg(score) as avgs from SC group by S# having avg(score) > 80) b where a.S#=b.S#

--3、查询所有同学的学号、姓名、选课数、总成绩； 
select a.S#, a.Sname, b.CourseCount, b.SumScore, b.AvgScore from Student a, 
(select S#, count(C#) as CourseCount, sum(score) as SumScore, avg(score) as AvgScore from SC group by S#) b
where a.S#=b.S#

--4、查询姓“李”的老师的个数；
select count(T#) from Teacher where Tname like 'Li %'

--5、查询没学过“叶平”老师课的同学的学号、姓名

--XXX!!!Wrong
select x.S#,x.Sname from Student x,
( select distinct(S#),C# from SC where C# not in (select distinct(a.C#) from SC a, Course b, Teacher c where a.C#=b.C# and b.T#=c.T# and c.Tname='Yang HL') ) y
where x.S#=y.S#

--right
   select Student.S#,Student.Sname 
    from Student  
    where S# not in (select distinct( SC.S#) from SC,Course,Teacher where  SC.C#=Course.C# and Teacher.T#=Course.T# and Teacher.Tname='Yang HL')

