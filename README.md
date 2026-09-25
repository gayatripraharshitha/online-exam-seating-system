# Online Exam Seating Arrangement System

A C-based application that automates examination center preference management and seat allocation for students.

## 📌 Project Overview

The **Online Exam Seating Arrangement System** is a Data Structures and C Programming project designed to automate the process of assigning students to examination centers and seats.

Students do not select individual seats. Instead, they submit a priority order of examination centers. The authority controls the preference window, approves preferences, generates the allocation, and locks the final seating arrangement.

The system also maintains student history and archives previous examination records before a new examination is created.

---

## 🎯 Objectives

- Automate examination seating allocation
- Allow students to submit examination-center priorities
- Prevent students from directly selecting seats
- Allocate seats according to submitted preferences
- Provide fallback allocation when preferred centers are full
- Maintain student-specific history
- Provide authority-controlled allocation
- Preserve previous examination records
- Support multiple examination cycles

---

## 👨‍🎓 Student Portal

Students can:

- Register with a Candidate ID and password
- Log in securely
- View available examination centers
- Submit three center preferences
- Update preferences while the preference window is open
- View their submitted preferences
- View their final examination allocation
- View their own history
- Log out

### Preference System

Students submit three priorities:

1. First preference
2. Second preference
3. Third preference

Students **cannot select an exact seat**.

The system decides the final seat based on:

- Student preferences
- Available capacity
- Reserve capacity
- System allocation rules

---

## 👨‍💼 Authority Portal

The examination authority can:

- Create examination centers
- View center information
- Open the preference window
- Close the preference window
- Approve submitted preferences
- Search candidates
- View audit history
- View system dashboard
- Generate seating allocation
- Lock the final allocation
- Archive the current examination
- Reset the system for a new examination

### Authority Login

**Username:** `admin`

**Password:** `admin123`

> These credentials are provided for academic demonstration purposes.

---

## 🧠 Allocation Algorithm

The allocation process follows a priority-based approach.

For each student:

```text
First Preference
       ↓
Is capacity available?
       ↓
     YES → Allocate
       ↓ NO
Second Preference
       ↓
Is capacity available?
       ↓
     YES → Allocate
       ↓ NO
Third Preference
       ↓
Is capacity available?
       ↓
     YES → Allocate
       ↓ NO
Fallback Center
       ↓
Any center with available usable capacity