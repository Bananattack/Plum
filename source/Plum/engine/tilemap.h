6#pragma once
namespace Plum
{
	class Tilemap
	{
		private:
			int width;
			int height;
			int* data;
			
		public:
			Tilemap(int width, int height)
			{
				this->width = width;
				this->height = height;
				data = new int[width * height];
			}

			int getWidth()
			{
				return width;
			}

			int getHeight()
			{
				return height;
			}

			int getTile(int tx, int ty)
			{
				if(tx < 0 || tx >= width || ty < 0 || ty >= height) return 0;
				return data[ty * height + tx];
			}

			void setTile(int tx, int ty, int tileIndex)
			{
				if(tx < 0 || tx >= width || ty < 0 || ty >= height) return;
				data[ty * height + tx] = tileIndex;
			}

			void rect(int tx, int ty, int tx2, int ty2, int tileIndex)
			{
				int i, j;
				if (tx > tx2)
				{
					PLUM_SWAP(tx, tx2);
				}
				if (ty > ty2)
				{
					PLUM_SWAP(ty, ty2);
				}
				// Don't draw if completely outside region.
				if(tx >= width || ty >= height || tx2 < 0 || ty2 < 0)
				{
					return;
				}
				// Keep rectangle inside region
				if(tx < 0)
				{
					tx = 0;
				}
				if(tx2 >= width)
				{
					tx2 = width - 1;
				}
				if(ty < 0)
				{
					ty = 0;
				}
				if(ty2 >= width)
				{
					ty2 = width - 1;
				}
				// Draw the horizontal lines of the rectangle.
				for(i = tx; i <= tx2; i++)
				{
					data[ty * width + i] = tileIndex;
					data[ty2 * width + i] = tileIndex;
				}
				// Draw the vertical lines of the rectangle.
				for(i = ty; i <= ty2; i++)
				{
					data[i * width + tx] = tileIndex;
					data[i * width + tx2] = tileIndex;
				}
			}

			void solidRect(int tx, int ty, int tx2, int ty2, int tileIndex)
			{
				int i, j;
				if (tx > tx2)
				{
					PLUM_SWAP(tx, tx2);
				}
				if (ty > ty2)
				{
					PLUM_SWAP(ty, ty2);
				}
				// Don't draw if completely outside region.
				if(tx >= width || ty >= height || tx2 < 0 || ty2 < 0)
				{
					return;
				}
				// Keep rectangle inside region
				if(tx < 0)
				{
					tx = 0;
				}
				ift(tx2 >= width)
				{
					tx2 = width - 1;
				}
				if(ty < 0)
				{
					ty = 0;
				}
				if(ty2 >= width)
				{
					ty2 = width - 1;
				}
				// Plot the solid rectangle
				for(i = ty; i <= ty2; i++)
				{
					for(j = tx; j <= tx2; j++)
					{
						data[i * width + j] = tileIndex;
					}
				}
			}

			void line(int tx, int ty, int tx2, int ty2, int tileIndex)
			{
				// Now we'll clip the line using Cohen-Sutherland clipping
				// (this source adapted from ika)
				int c1 = 0;
				int c2 = 0;

				// Silly region code things to start us off
				if(tx < 0)	c1 |= 1;
				if(tx >= width)	c1 |= 2;
				if(ty < 0)	c1 |= 4;
				if(ty >= height)	c1 |= 8;
				if(tx2 < 0)	c2 |= 1;
				if(tx2 >= width)	c2 |= 2;
				if(ty2 < 0)	c2 |= 4;
				if(ty2 >= height)	c2 |= 8;

				// Keep clipping until we either accept or reject this line.
				while((c1 & c2) == 0 && (c1 | c2) != 0)
				{
					if(c1)
					{
						// Clip left.
						if(c1 & 1)
						{
							ty -= ((ty - ty2) * (0 - tx)) / (tx2 - tx + 1);
							tx = 0;
						}
						// Clip right.
						if(c1 & 2)
						{
							ty -= ((ty - ty2) * (tx - width - 1)) / (tx2 - tx + 1);
							tx = width - 1;
						}
						// Clip above.
						else if(c1 & 4)
						{
							tx -= ((tx - tx2) * (0 - ty)) / (ty2 - ty + 1);
							ty = 0;
						}
						// Clip below.
						else
						{
							tx -= ((tx - tx2) * (ty - height - 1)) / (ty2 - ty + 1);
							ty = height - 1;
						}
						// Recheck all the region codes for this side of the line.
						c1 = 0;
						if(tx < 0)	c1 |= 1;
						if(tx >= width)	c1 |= 2;
						if(ty < 0)	c1 |= 4;
						if(ty >= height)	c1 |= 8;
					}
					else
					{
						// Clip left.
						if(c2 & 1)
						{
							ty2 -= ((ty2 - ty) * (0 - tx2)) / (tx - tx2 + 1);
							tx2 = 0;
						}
						// Clip right.
						if(c2 & 2)
						{
							ty2 -= ((ty2 - ty) * (tx2 - width - 1)) / (tx - tx2 + 1);
							tx2 = width - 1;
						}
						// Clip above.
						else if(c2 & 4)
						{
							tx2 -= ((tx2 - tx) * (0 - ty2)) / (ty - ty2 + 1);
							ty2 = 0;
						}
						// Clip below.
						else
						{
							tx2 -= ((tx2 - tx) * (ty2 - height - 1)) / (ty - ty2 + 1);
							ty2 = height;
						}
						// Recheck all the region codes for this side of the line.
						c2 = 0;
						if(tx2 < 0)	c2 |= 1;
						if(tx2 >= width)	c2 |= 2;
						if(ty2 < 0)	c2 |= 4;
						if(ty2 >= height)	c2 |= 8;
					}
				}
				// Re-jected!
				if(c1 & c2) return;
				// A single pixel
				if(tx == tx2 && ty == ty2)
				{
					data[ty * width + tx] = tileIndex;
					return;
				}
				// Horizontal line
				if(ty == ty2)
				{
					// Put the coordinates in order.
					if (tx > tx2)
					{
						PLUM_SWAP(tx, tx2);
					}
					// Draw it.
					for(int i = tx; i <= tx2; i++)
					{
						data[ty * width + i] = tileIndex;
					}
					return;
				}
				// Vertical line
				if(tx == tx2)
				{
					// Put the coordinates in order.
					if (ty > ty2)
					{
						PLUM_SWAP(ty, ty2);
					}
					// Draw it.
					for(int i = ty; i <= ty2; i++)
					{
						data[i * width + tx] = tileIndex;
					}
					return;
				}
				// Time for some Bresenham! (Thanks Kildorf, and Mr. Jack E. Bresenham)
				int cx, cy, dx, dy;
				int xaccum, yaccum, xincre, yincre, xreset, yreset, xchange, ychange;
				bool done = false;

				dx = abs(tx2 - tx);
				dy = abs(ty2 - ty);
				cx = tx;
				cy = ty;
				xchange = (tx > tx2) ? -1 : 1;
				ychange = (ty > ty2) ? -1 : 1;

				if(dx > dy)
				{
					xaccum = 0;
					xreset = 0;
					xincre = 0;
					yaccum = dy * 2 - dx;
					yincre = dy * 2;
					yreset = (dy - dx) * 2;
				}
				else
				{
					yaccum = 0;
					yreset = 0;
					yincre = 0;
					xaccum = dx * 2 - dy;
					xincre = dx * 2;
					xreset = (dx - dy) * 2;
				}

				while(!done)
				{
					if(xaccum < 0)
					{
						xaccum += xincre;
					}
					else
					{
						cx += xchange;
						xaccum += xreset;
					}

					if(yaccum < 0)
					{
						yaccum += yincre;
					}
					else
					{
						cy += ychange;
						yaccum += yreset;
					}

					data[cy * width + cx] = tileIndex;

					if(xreset == 0 && cx == tx2) done = true;
					if(yreset == 0 && cy == ty2) done = true;
				}
			}

			void stamp(int tx, int ty, TileMap* dest)
			{
				int i, j;
				int tx2 = tx + width - 1;
				int ty2 = ty + height -1;
				int sourceX = 0;
				int sourceY = 0;
				int sourceX2 = width - 1;
				int sourceY2 = height - 1;

				// Don't plot if completely outside dest region.
				if(tx >= dest->width || ty >= dest->height || tx2 < 0 || ty2 < 0)
				{
					return;
				}
				// Keep rectangle inside dest region
				if(tx < 0)
				{
					sourceX += 0 - tx;
				}
				if(tx2 >= dest->width)
				{
					sourceX2 -= tx2 - dest->width - 1;
				}
				if(ty < 0)
				{
					sourceY += 0 - ty;
				}
				if(tx2 >= dest->height)
				{
					sourceY2 -= ty2 - dest->height - 1;
				}
				// Plot the tilemap, tile for tile
				for(i = sourceY; i <= sourceY2; i++)
				{
					for(j = sourceX; j <= sourceX2; j++)
					{
						dest->data[(i + ty) * dest->width + (j + tx)] = data[i * width + j];
					}
				}
			}
	}
}