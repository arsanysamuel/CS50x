SELECT title FROM movies WHERE id IN (SELECT DISTINCT johnny.movie_id FROM stars AS johnny JOIN stars AS bonham ON johnny.movie_id = bonham.movie_id WHERE johnny.person_id = (SELECT id FROM people WHERE name = "Johnny Depp") AND bonham.person_id = (SELECT id FROM people WHERE name = "Helena Bonham Carter"));
