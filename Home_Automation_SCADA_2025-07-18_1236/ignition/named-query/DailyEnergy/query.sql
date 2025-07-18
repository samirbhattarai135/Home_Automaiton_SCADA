SELECT
    DATE(t_stamp) AS day,
    AVG(CASE WHEN tagid = (SELECT id FROM sqlt_tags WHERE tagpath = 'Home/Bedroom/Temperature_C') THEN f_float ELSE NULL END) AS Bedroom_Temperature_C,
    AVG(CASE WHEN tagid = (SELECT id FROM sqlt_tags WHERE tagpath = 'Home/Kitchen/Humidity_') THEN f_float ELSE NULL END) AS Kitchen_Humidity
FROM
    sqlt_data_1_2025_07 -- Adjust table name if needed for other months/years
WHERE
    t_stamp BETWEEN :startDate AND :endDate
    AND tagid IN (
        SELECT id FROM sqlt_tags WHERE tagpath = 'Home/Bedroom/Temperature_C'
        UNION ALL
        SELECT id FROM sqlt_tags WHERE tagpath = 'Home/Kitchen/Humidity_'
    )
GROUP BY
    DATE(t_stamp)
ORDER BY
    day