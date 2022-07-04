-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Obtain the crime scene report of the theft
SELECT * FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- Theft Description: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--                    Interviews were conducted today with three witnesses who were present at the time –
--                    each of their interview transcripts mentions the bakery.

-- Let us check the interviews on the theft date that mention the bakery
SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;

-- The 3 witnesses are Ruth, Eugene and Raymond
-- Ruth: the thief leaves the bakery +- 10 minutes from the theft time of 10:15
-- Eugene: the thief withdrew money from the ATM on Leggett Street earlier in the morning before he arrived at the bakery
-- Raymond: the thief call his accomplice for < 1 minute as he left the bakery, with the accomplice buying the earliest tickets out of
--          fiftyville on 27/7/2021

-- Find who withdrew money from the ATM on Leggett Street on 28/7/2021
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';

-- Find people who left the bakery between 10:05 and 10:25
SELECT name FROM people
JOIN bakery_security_logs as bk ON people.license_plate = bk.license_plate
WHERE bk.year = 2021 AND bk.month = 7 AND bk.day = 28 AND bk.hour = 10 AND bk.minute >= 5 AND bk.minute <= 25 AND bk.activity = 'exit';

-- Find people who made < 1 minute calls on 28/7/2021
SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.duration < 60 AND phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28;

-- The common names of these 3 lists are Bruce and Diana

-- Find the first flight out of Fiftyville on 29/7/2021, order by time
SELECT flights.id FROM flights
WHERE flights.origin_airport_id = (
    SELECT id FROM airports
    WHERE city = 'Fiftyville'
)
AND flights.year = 2021 AND flights.month = 7 and flights.day = 29
ORDER BY flights.hour, flights.minute;

-- Check the passengers on the first flight from Fiftyville to New York City (flight id = 36)
SELECT name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE passengers.flight_ID = 36;

-- The only common name is Bruce - he is the thief

-- Check who Bruce called to find the accomplice
SELECT people.name FROM people
WHERE people.phone_number = (
    SELECT phone_calls.receiver FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.caller
    WHERE people.name = 'Bruce' AND year = 2021 AND month = 7 AND day = 28 AND duration < 60
);

-- Therefore the accomplice is Robin
