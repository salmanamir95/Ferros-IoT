import { useMemo } from 'react';
import type { FerrosInsight } from '../types/ferros';
import { AreaChart, Area, XAxis, YAxis, Tooltip, ResponsiveContainer, CartesianGrid } from 'recharts';

interface Props {
  events: FerrosInsight[];
}

export const MetricsPanel: React.FC<Props> = ({ events }) => {
  const chartData = useMemo(() => {
    // Only use events that have some numerical metrics
    return events
      .filter(e => e.metrics && (e.metrics.pressure_score !== undefined || e.metrics.fragmentation_rate !== undefined))
      .slice(-50)
      .map(e => {
        const d = new Date(e.trace.timestamp_ns / 1000000);
        return {
          time: `${d.getSeconds()}.${String(d.getMilliseconds()).padStart(3, '0')}`,
          pressure: e.metrics.pressure_score || 0,
          fragmentation: e.metrics.fragmentation_rate || 0,
        };
      });
  }, [events]);

  return (
    <div className="glass-panel" style={{ padding: '1.5rem', display: 'flex', flexDirection: 'column', gap: '1rem' }}>
      <h2 style={{ fontSize: '1.25rem', fontWeight: 600 }}>System Metrics Timeline</h2>
      
      <div style={{ flex: 1, minHeight: '250px' }}>
        {chartData.length > 0 ? (
          <ResponsiveContainer width="100%" height="100%">
            <AreaChart data={chartData} margin={{ top: 10, right: 10, left: -20, bottom: 0 }}>
              <defs>
                <linearGradient id="colorPressure" x1="0" y1="0" x2="0" y2="1">
                  <stop offset="5%" stopColor="var(--neon-red)" stopOpacity={0.8}/>
                  <stop offset="95%" stopColor="var(--neon-red)" stopOpacity={0}/>
                </linearGradient>
                <linearGradient id="colorFrag" x1="0" y1="0" x2="0" y2="1">
                  <stop offset="5%" stopColor="var(--neon-cyan)" stopOpacity={0.8}/>
                  <stop offset="95%" stopColor="var(--neon-cyan)" stopOpacity={0}/>
                </linearGradient>
              </defs>
              <CartesianGrid strokeDasharray="3 3" stroke="rgba(255,255,255,0.1)" vertical={false} />
              <XAxis dataKey="time" stroke="var(--text-muted)" fontSize={12} tickLine={false} axisLine={false} />
              <YAxis stroke="var(--text-muted)" fontSize={12} tickLine={false} axisLine={false} />
              <Tooltip 
                contentStyle={{ backgroundColor: 'var(--bg-panel)', borderColor: 'var(--border-light)', borderRadius: '8px' }}
                itemStyle={{ color: 'var(--text-main)' }}
              />
              <Area type="monotone" dataKey="pressure" stroke="var(--neon-red)" fillOpacity={1} fill="url(#colorPressure)" />
              <Area type="monotone" dataKey="fragmentation" stroke="var(--neon-cyan)" fillOpacity={1} fill="url(#colorFrag)" />
            </AreaChart>
          </ResponsiveContainer>
        ) : (
          <div style={{ height: '100%', display: 'flex', alignItems: 'center', justifyContent: 'center', color: 'var(--text-muted)' }}>
            Not enough metric data yet.
          </div>
        )}
      </div>
    </div>
  );
};
