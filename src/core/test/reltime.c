

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>

#include <check.h>

#include <Reltime.h>


Suite* Reltime_suite(void);


START_TEST (init)
{
	Reltime rel;
	Reltime* rp = Reltime_init(&rel);
	fail_unless(rp == &rel,
			"Reltime_init() returned %p instead of %p.", rp, &rel);
	fail_unless(rp->beats == 0,
			"Reltime_init() set beats to %lld instead of 0.", (long long)rp->beats);
	fail_unless(rp->part == 0,
			"Reltime_init() set part to %ld instead of 0.", (long)rp->part);
}
END_TEST

#ifndef NDEBUG
START_TEST (init_break)
{
	Reltime_init(NULL);
}
END_TEST
#endif

START_TEST (set)
{
	int64_t beat_values[] = { INT64_MIN, INT64_MIN + 1, -1, 0, 1, INT64_MAX - 1, INT64_MAX };
	int32_t part_values[] = { 0, 1, RELTIME_FULL_PART - 1 };
	int32_t all_parts[] = { INT32_MIN, INT32_MIN + 1, -1, 0, 1,
			RELTIME_FULL_PART - 1, RELTIME_FULL_PART, INT32_MAX - 1, INT32_MAX };
	for (size_t i = 0; i < sizeof(beat_values) / sizeof(int64_t); ++i)
	{
		for (size_t k = 0; k < sizeof(part_values) / sizeof(int32_t); ++k)
		{
			for (size_t l = 0; l < sizeof(all_parts) / sizeof(int32_t); ++l)
			{
				Reltime* r = Reltime_init(&(Reltime){ .beats = 0 });
				r->part = all_parts[l];
				Reltime* s = Reltime_set(r, beat_values[i], part_values[k]);
				fail_unless(s == r,
						"Reltime_set() returned %p instead of %p.", s, r);
				fail_unless(s->beats == beat_values[i],
						"Reltime_set() set beats to %lld instead of %lld.",
						(long long)s->beats, (long long)beat_values[i]);
				fail_unless(s->part == part_values[k],
						"Reltime_set() set part to %lld instead of %lld.",
						(long long)s->part, (long long)part_values[k]);
			}
		}
	}
}
END_TEST

#ifndef NDEBUG
START_TEST (set_break_reltime)
{
	Reltime_set(NULL, 0, 0);
}
END_TEST

START_TEST (set_break_part1)
{
	Reltime_set(&(Reltime){ .beats = 0 }, 0, INT32_MIN);
}
END_TEST

START_TEST (set_break_part2)
{
	Reltime_set(&(Reltime){ .beats = 0 }, 0, -1);
}
END_TEST

START_TEST (set_break_part3)
{
	Reltime_set(&(Reltime){ .beats = 0 }, 0, RELTIME_FULL_PART);
}
END_TEST

START_TEST (set_break_part4)
{
	Reltime_set(&(Reltime){ .beats = 0 }, 0, INT32_MAX);
}
END_TEST
#endif

START_TEST (cmp)
{
	Reltime* r1 = Reltime_init(&(Reltime){ .beats = 0 });
	Reltime* r2 = Reltime_init(&(Reltime){ .beats = 0 });
	int res = 0;
#define CMPTEXT(c) ((c) < 0 ? "smaller" : ((c) > 0 ? "greater" : "equal"))

	// beats and parts equal
	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MIN, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MIN, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	
	Reltime_set(r1, INT64_MIN + 1, 0);
	Reltime_set(r2, INT64_MIN + 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN + 1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MIN + 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	
	Reltime_set(r1, -1, 0);
	Reltime_set(r2, -1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, -1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, -1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	
	Reltime_set(r1, 0, 0);
	Reltime_set(r2, 0, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, 0, RELTIME_FULL_PART - 1);
	Reltime_set(r2, 0, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));

	Reltime_set(r1, 1, 0);
	Reltime_set(r2, 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, 1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));

	Reltime_set(r1, INT64_MAX - 1, 0);
	Reltime_set(r2, INT64_MAX - 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, INT64_MAX - 1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MAX - 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));

	Reltime_set(r1, INT64_MAX, 0);
	Reltime_set(r2, INT64_MAX, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));
	Reltime_set(r1, INT64_MAX, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res == 0,
			"Reltime_cmp() returned %s instead of equal.", CMPTEXT(res));

	// beats equal and parts unequal
	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MIN, 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MIN, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, RELTIME_FULL_PART - 2);
	Reltime_set(r2, INT64_MIN, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, -1, 0);
	Reltime_set(r2, -1, 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, -1, 0);
	Reltime_set(r2, -1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, -1, RELTIME_FULL_PART - 2);
	Reltime_set(r2, -1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, 0, 0);
	Reltime_set(r2, 0, 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, 0, 0);
	Reltime_set(r2, 0, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, 0, RELTIME_FULL_PART - 2);
	Reltime_set(r2, 0, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, 1, 0);
	Reltime_set(r2, 1, 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, 1, 0);
	Reltime_set(r2, 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, 1, RELTIME_FULL_PART - 2);
	Reltime_set(r2, 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, INT64_MAX, 0);
	Reltime_set(r2, INT64_MAX, 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MAX, 0);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MAX, RELTIME_FULL_PART - 2);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	// beats unequal and parts equal
	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MIN + 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MIN + 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, -1, 0);
	Reltime_set(r2, 0, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, -1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, 0, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, 0, 0);
	Reltime_set(r2, 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, 0, RELTIME_FULL_PART - 1);
	Reltime_set(r2, 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, INT64_MAX - 1, 0);
	Reltime_set(r2, INT64_MAX, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MAX - 1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MAX, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));

	// beats and parts unequal
	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MIN + 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MIN + 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	
	Reltime_set(r1, -1, 0);
	Reltime_set(r2, 0, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, -1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, 0, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	
	Reltime_set(r1, 0, 0);
	Reltime_set(r2, 1, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, 0, RELTIME_FULL_PART - 1);
	Reltime_set(r2, 1, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	
	Reltime_set(r1, INT64_MAX - 1, 0);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MAX - 1, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MAX, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	
	Reltime_set(r1, INT64_MIN, 0);
	Reltime_set(r2, INT64_MAX, RELTIME_FULL_PART - 1);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
	Reltime_set(r1, INT64_MIN, RELTIME_FULL_PART - 1);
	Reltime_set(r2, INT64_MAX, 0);
	res = Reltime_cmp(r1, r2);
	fail_unless(res < 0,
			"Reltime_cmp() returned %s instead of smaller.", CMPTEXT(res));
	res = Reltime_cmp(r2, r1);
	fail_unless(res > 0,
			"Reltime_cmp() returned %s instead of greater.", CMPTEXT(res));
}
END_TEST

#ifndef NDEBUG
START_TEST (cmp_break_null1)
{
	Reltime_cmp(NULL, Reltime_init(&(Reltime){ .beats = 0 }));
}
END_TEST

START_TEST (cmp_break_null2)
{
	Reltime_cmp(Reltime_init(&(Reltime){ .beats = 0 }), NULL);
}
END_TEST

START_TEST (cmp_break_inv11)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = INT32_MIN };
	Reltime_cmp(br, Reltime_init(&(Reltime){ .beats = 0 }));
}
END_TEST

START_TEST (cmp_break_inv12)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = -1 };
	Reltime_cmp(br, Reltime_init(&(Reltime){ .beats = 0 }));
}
END_TEST

START_TEST (cmp_break_inv13)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = RELTIME_FULL_PART };
	Reltime_cmp(br, Reltime_init(&(Reltime){ .beats = 0 }));
}
END_TEST

START_TEST (cmp_break_inv14)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = INT32_MAX };
	Reltime_cmp(br, Reltime_init(&(Reltime){ .beats = 0 }));
}
END_TEST

START_TEST (cmp_break_inv21)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = INT32_MIN };
	Reltime_cmp(Reltime_init(&(Reltime){ .beats = 0 }), br);
}
END_TEST

START_TEST (cmp_break_inv22)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = -1 };
	Reltime_cmp(Reltime_init(&(Reltime){ .beats = 0 }), br);
}
END_TEST

START_TEST (cmp_break_inv23)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = RELTIME_FULL_PART };
	Reltime_cmp(Reltime_init(&(Reltime){ .beats = 0 }), br);
}
END_TEST

START_TEST (cmp_break_inv24)
{
	Reltime* br = &(Reltime){ .beats = 0, .part = INT32_MAX };
	Reltime_cmp(Reltime_init(&(Reltime){ .beats = 0 }), br);
}
END_TEST
#endif


Suite* Reltime_suite(void)
{
	Suite* s = suite_create("Reltime");
	TCase* tc_init = tcase_create("init");
	TCase* tc_set = tcase_create("set");
	TCase* tc_cmp = tcase_create("cmp");
	suite_add_tcase(s, tc_init);
	suite_add_tcase(s, tc_set);
	suite_add_tcase(s, tc_cmp);

	tcase_add_test(tc_init, init);
	tcase_add_test(tc_set, set);
	tcase_add_test(tc_cmp, cmp);

#ifndef NDEBUG
	tcase_add_test_raise_signal(tc_init, init_break, SIGABRT);

	tcase_add_test_raise_signal(tc_set, set_break_reltime, SIGABRT);
	tcase_add_test_raise_signal(tc_set, set_break_part1, SIGABRT);
	tcase_add_test_raise_signal(tc_set, set_break_part2, SIGABRT);
	tcase_add_test_raise_signal(tc_set, set_break_part3, SIGABRT);
	tcase_add_test_raise_signal(tc_set, set_break_part4, SIGABRT);

	tcase_add_test_raise_signal(tc_cmp, cmp_break_null1, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_null2, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv11, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv12, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv13, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv14, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv21, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv22, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv23, SIGABRT);
	tcase_add_test_raise_signal(tc_cmp, cmp_break_inv24, SIGABRT);
#endif

	return s;
}


int main(void)
{
	int fail_count = 0;
	Suite* s = Reltime_suite();
	SRunner* sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	fail_count = srunner_ntests_failed(sr);
	srunner_free(sr);
	if (fail_count > 0)
	{
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}


