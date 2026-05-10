export interface FerrosIdentity {
  pid: number;
  ppid: number;
  tgid: number;
  comm: string;
  scope: string;
}

export interface FerrosTrace {
  cpu: number;
  event_type: string;
  timestamp_ns: number;
}

export interface FerrosMetrics {
  churn?: number;
  fragmentation_rate?: number;
  cpu?: number;
  entropy?: number;
  migration_rate?: number;
  pressure_score?: number;
  switch_rate?: number;
  hot_pid?: number;
  max_density?: number;
  active_threads?: number;
  fairness_score?: number;
  pid?: number;
  burst_switches?: number;
  window_ms?: number;
}

export interface FerrosInsight {
  correlation_id: string;
  identity: FerrosIdentity;
  trace: FerrosTrace;
  metrics: FerrosMetrics;
  type: string;
}
