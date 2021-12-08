-- Keep a log of any SQL queries you execute as you solve the mystery.

.schema

-- So, I start looking at the crime scene report for given date and place:

SELECT description FROM crime_scene_reports 
WHERE month = "7" AND day = "28" AND street = "Chamberlin Street";

-- "Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. 
-- Interviews were conducted today with three witnesses who were present at the time
-- each of their interview transcripts mentions the courthouse."

-- Next i will check interviews of witnesses, for given date:

SELECT name, transcript FROM interviews WHERE month = "7" AND day = "28";

-- Ruth: saw thief getting into car at courthouse parking lot within 10 minutes of the theft
-- Eugene: said thiefe was familiar to him, saw him getting money from ATM earlier that morning on Fifer Street
-- Raymond: Heard that thief was talking to someone by phone (less then minute), 
-- regarding they should take earliest flight out of town tomorrow, and asked person they were talking to, to buy tickets.

-- Next i wanna check security logs at Courthouse parking lot for 10:15 + 10 minutes

SELECT activity, license_plate, hour, minute FROM courthouse_security_logs 
WHERE year = "2020" AND month = "7" AND day = "28" 
AND hour = "10" AND minute > "15" AND minute < "25";

-- So, within 10 min of the theft following cars left parking lot, on the right side is their arrival time:

-- plate     h    m   (entry)
-- 5P2BI95 | 10 | 16 - 9:15
-- 94KL13X | 10 | 18 - 8:23
-- 6P58WS2 | 10 | 18 - 9:20
-- 4328GD8 | 10 | 19 - 9:14
-- G412CB7 | 10 | 20 - 9:28
-- L93JTIZ | 10 | 21 - 8:18
-- 322W7JE | 10 | 23 - 8:36
-- 0NTHK55 | 10 | 23 - 8:42

-- Eugene said that he saw thief using ATM before he arrived in, so ill check logs
SELECT * FROM atm_transactions WHERE day = "28"
AND month = "7" AND year = "2020" AND atm_location = "Fifer Street";

-- id | account_number | year | month | day | atm_location | transaction_type | amount
-- 246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
-- 264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
-- 267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
-- 269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
-- 275 | 86363979 | 2020 | 7 | 28 | Fifer Street | deposit | 10
-- 288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
-- 336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35

-- Lets check if we have intersection between cars that left courthouse in given time,
-- with acccount number involved in operation during that day

SELECT id, license_plate, passport_number, phone_number, name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE day = "28"
AND month = "7" AND year = "2020" AND atm_location = "Fifer Street"));

-- Below are names and other data for people that used ATM during time we are interested in:

-- id | license_plate | passport_number | phone_number | name
-- 395717 | 30G67EN | 9878712108 | (826) 555-1652 | Bobby
-- 396669 | L93JTIZ | 7049073643 | (829) 555-5269 | Elizabeth
-- 438727 | 8X428L0 | 9586786673 | (338) 555-6650 | Victoria
-- 449774 | 1106N58 | 1988161715 | (286) 555-6063 | Madison
-- 458378 | QX4YZN3 | 4408372428 | (122) 555-4581 | Roy
-- 467400 | 4328GD8 | 8496433585 | (389) 555-5198 | Danielle
-- 514354 | 322W7JE | 3592750733 | (770) 555-1861 | Russell
-- 686048 | 94KL13X | 5773159633 | (367) 555-5533 | Ernest
-- 948985 | I449449 | 8304650265 | (098) 555-1164 | Robert

-- now i wanna see if we have same license plate

SELECT license_plate FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE day = "28"
AND month = "7" AND year = "2020" AND atm_location = "Fifer Street"))
INTERSECT
SELECT license_plate FROM courthouse_security_logs 
WHERE year = "2020" AND month = "7" AND day = "28" 
AND hour = "10" AND minute > "15" AND minute < "25";

-- we have 4 intersection:
-- license_plate

-- 322W7JE
-- 4328GD8
-- 94KL13X
-- L93JTIZ

-- Fo qol ill put them in more comfortable view:
SELECT id, license_plate, passport_number, phone_number, name FROM people 
WHERE license_plate IN ("L93JTIZ", "4328GD8", "322W7JE", "94KL13X");

-- So here are our suspects!

-- id | license_plate | passport_number | phone_number | name
-- 396669 | L93JTIZ | 7049073643 | (829) 555-5269 | Elizabeth
-- 467400 | 4328GD8 | 8496433585 | (389) 555-5198 | Danielle (probably not a thief, because of ATM operation)
-- 514354 | 322W7JE | 3592750733 | (770) 555-1861 | Russell
-- 686048 | 94KL13X | 5773159633 | (367) 555-5533 | Ernest

-- we can exclude Danielle, because she was only one who deposited in ATM that day.

-- Now it's time to check their phones, thiefe called someone and talked less than 1 minute.
SELECT * FROM phone_calls WHERE day = "28" AND caller IN
(SELECT phone_number FROM people 
WHERE license_plate IN ("L93JTIZ", "4328GD8", "322W7JE", "94KL13X"));

-- id | caller | receiver | year | month | day | duration
-- 233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
-- 236 | (367) 555-5533 | (344) 555-9601 | 2020 | 7 | 28 | 120
-- 245 | (367) 555-5533 | (022) 555-4052 | 2020 | 7 | 28 | 241
-- 255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
-- 285 | (367) 555-5533 | (704) 555-5790 | 2020 | 7 | 28 | 75

-- we can see that Ernest and Russell both called someone and talked less than a minute.
-- updating suspect list:

-- id | license_plate | passport_number | phone_number | name
-- 514354 | 322W7JE | 3592750733 | (770) 555-1861 | Russell
-- 686048 | 94KL13X | 5773159633 | (367) 555-5533 | Ernest

-- lets check if those guys has left the city (thief said he wants to take earliest flight tomorrow from fiftyville)

SELECT * FROM flights WHERE id IN
(SELECT flight_id FROM passengers WHERE passport_number IN ("3592750733", "5773159633"));

-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0
-- 24 | 7 | 8 | 2020 | 7 | 30 | 16 | 27
-- 36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
-- 54 | 8 | 5 | 2020 | 7 | 30 | 10 | 19

-- we can see that earliest flight id for 29th was #36, looks like it was Ernest! 
-- lets check what are Origin and Destination airports for that light:

SELECT * FROM airports WHERE id IN ("8", "4");

-- id | abbreviation | full_name | city
-- 4 | LHR | Heathrow Airport | London
-- 8 | CSF | Fiftyville Regional Airport | Fiftyville

-- Looks like Mr. Ernest flew to London.

-- Now lets check who helped him to buy ticket, first we track number ((375) 555-8161) he called that day with duration less than minute.

SELECT * FROM people WHERE phone_number = "(375) 555-8161";

-- id | name | phone_number | passport_number | license_plate
-- 864400 | Berthold | (375) 555-8161 |  | 4V16VO0

-- shady guy, with no passpot? lets check bank account:

SELECT * FROM bank_accounts WHERE person_id IN (SELECT id FROM people WHERE phone_number = "(375) 555-8161");

-- account_number | person_id | creation_year
-- 94751264 | 864400 | 2019

-- it seem there is not enough info to prove that Berthold is indeed guilty though ;)

-- CONCLUSION: Mr. Ernest, stole the duck on 28th of July 2020. 
-- He arrived at courthouse parking lot on vehicle with license plate (94KL13X) at 8:23 and left at 10:18
-- Same day was seen withdrawing 30$ from ATM by witness who recognized him as familiar face.
-- Presumably asked his accomplice by phone, to book him earliest flight on 29th of July.
-- He left Fiftyville using early flight to London 29.07.2020 at 08:20.